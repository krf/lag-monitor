#include "CanvasWidget.h"

#include "Utils.h"
#include "sources/Source.h"

#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QTime>

#include <algorithm>

// constants
static const QSize CANVAS_MIN_SIZE(100, 40);
static const int CANVAS_FPS = 1; // frames per second

static const QColor DEFAULT_BACKGROUND_COLOR(255, 255, 255);

static bool compare_delay(const Pong& p1, const Pong p2)
{
    return p1.delay < p2.delay;
}

CanvasWidget::CanvasWidget(QWidget* parent, Qt::WindowFlags f)
    : QFrame(parent, f)
    , m_updateTimer(new QTimer(this))
{
    // main paint event driver
    m_updateTimer->setInterval(1000.0/CANVAS_FPS);
    m_updateTimer->start();
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(update()));

    // lower point size of the font
    QFont font = this->font();
    font.setPointSize(8);
    setFont(font);

    // dark bg
    QPalette palette = this->palette();
    palette.setColor(backgroundRole(), Qt::black);
    setPalette(palette);
    setAutoFillBackground(true);
}

void CanvasWidget::setSource(Source* source)
{
    debug() << source;

    if (m_source) {
        disconnect(m_source, SIGNAL(pongReceived(Pong)), this, SLOT(handlePongReceived(Pong)));
    }
    m_source = source;
    if (m_source) {
        connect(m_source, SIGNAL(pongReceived(Pong)), SLOT(handlePongReceived(Pong)));
    }
}

void CanvasWidget::setTimeSpan(int timeSpan)
{
    m_timeSpan = qMax(5000, timeSpan);
}

void CanvasWidget::handlePongReceived(const Pong& pong)
{
    m_history.enqueue(pong);

    // clean up old events
    int count = 0;
    while(m_history.size() != 0 && m_history.head().time.elapsed() > m_timeSpan) {
        m_history.dequeue();
        ++count;
    }

    // update statistical values
    if (!m_history.isEmpty())
        m_maxDelay = (*std::max_element(m_history.begin(), m_history.end(), compare_delay)).delay;
    else
        m_maxDelay = -1;

    //debug() << "Cleared" << count << "items from cache";
}

void CanvasWidget::paintEvent(QPaintEvent* e)
{
    if (!source()) {
        QFrame::paintEvent(e);
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    paintBackground(&painter);
    painter.restore();
    painter.save();
    paintForeground(&painter);
    painter.restore();

    QFrame::paintEvent(e);
}

void CanvasWidget::paintBackground(QPainter* painter) const
{
    const QRect r = rect();
    painter->setPen(QColor(255,255,255,200));
    painter->drawRect(0, 0, r.width(), r.height());
}

void CanvasWidget::paintForeground(QPainter* painter) const
{
    if (m_history.isEmpty())
        return;

    // constants
    static const int topMarginY = 10;
    static const int marginX = 5;

    const int& maxDelay = m_maxDelay;
    const int minDelay = 0;
    const int midDelay = (maxDelay - minDelay) / 2.0;

    const QRect r = rect();
    const int width = r.width();
    const int height = r.height();

    const QFontMetrics fm = fontMetrics();

    painter->setPen(QPen(QColor(255,255,255,200), 1, Qt::DotLine));

    // draw y axis label for maximum value
    {
    const int yPos = topMarginY;
    painter->drawText(QPoint(marginX, yPos + fm.ascent()), tr("%1 ms (max)").arg(maxDelay));
    painter->drawLine(0, yPos, width, yPos);
    }

    // draw y axis label for mid value (if possible)
    if (height > 60) {
        const int yPos = (height + topMarginY) / 2.0;
        painter->drawText(QPoint(marginX, yPos + fm.ascent()), tr("%1 ms (mid)").arg(midDelay));
        painter->drawLine(0, yPos, width, yPos);
    }

    // draw x axis labels (if possible)
    if (height > 30) {
        {
        static QString nowText = tr("now |");
        const int nowTextWidth = fm.width(nowText);
        painter->drawText(QPoint(width - nowTextWidth - 2, height - marginX), nowText);
        }

        {
        const QString agoText = tr("%1s ago |").arg(m_timeSpan / 1000 / 2);
        const int agoTextWidth = fm.width(agoText);
        painter->drawText(QPoint(width/2.0 - agoTextWidth, height - marginX), agoText);
        }
    }

    const float xScale = width / (float)m_timeSpan;
    const float yScale = (height - topMarginY) / (float)(maxDelay == 0 ? 1 : maxDelay);

    QPolygon polygon;
    Q_FOREACH(const Pong& pong, m_history) {
        const int elapsed = pong.time.elapsed();
        // ignore events that are older than X ms
        if (elapsed > m_timeSpan) {
            continue;
        }

        const int& xValue = elapsed;
        const int& yValue = pong.delay;
        const int xPos = (width) - xValue*xScale;
        const int yPos = (height) - yValue*yScale;
        polygon << QPoint(xPos, yPos);
    }

    // draw red box when no pongs occurred for some time
    const Pong& lastPong = m_history.back();
    {
        const int elapsed = lastPong.time.elapsed();
        if (elapsed > 5000) {
            static const QColor color(255, 0, 0, 120);

            const int& xValue = elapsed;
            const int xPos = (width) - xValue*xScale;
            painter->save();
            painter->setBrush(color);
            painter->drawRect(xPos, 0, width - xPos, height);
            painter->restore();
        }
    }

    painter->setPen(QPen(Qt::gray, 1));
    painter->drawPolyline(polygon);
    painter->setPen(QPen(Qt::white, 2));
    painter->drawPoints(polygon);
}

QSize CanvasWidget::minimumSizeHint() const
{
    return CANVAS_MIN_SIZE;
}

