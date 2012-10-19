#include "MonitorWidget.h"

#include "CanvasWidget.h"
#include "Source.h"
#include "Utils.h"

#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QTimer>

static const int JUST_NOW_THRESHOLD = 1000; // ms

static QString formattedTime(int ms, int precision = 2)
{
    if (ms > 1000)
        return QObject::tr("%1 secs").arg(QString::number(ms/1000.0, 'f', precision));
    else
        return QObject::tr("%1 ms").arg(QString::number(ms));
}

MonitorWidget::MonitorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_source(0)
    , m_fallBackUpdateTimer(new QTimer(this))
    , m_updateTimer(new QTimer(this))
    , m_label(new QLabel)
    , m_canvasWidget(new CanvasWidget)
{
    m_label->setMargin(5);
    m_canvasWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(m_label);
    vbox->addWidget(m_canvasWidget);

    m_updateTimer->setInterval(1000);
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(updateLabel()));

    m_fallBackUpdateTimer->setInterval(10000);
    m_fallBackUpdateTimer->setSingleShot(true);
    connect(m_fallBackUpdateTimer, SIGNAL(timeout()), m_updateTimer, SLOT(start()));

    updateLabel();
}

MonitorWidget::~MonitorWidget()
{
    debug();
}

void MonitorWidget::setSource(Source* source)
{
    if (m_source) {
        disconnect(m_source, SIGNAL(updateIntervalChanged(int)), this, SLOT(handleUpdateIntervalChanged(int)));
        disconnect(m_source, SIGNAL(pongReceived(Pong)), this, SLOT(handlePongReceived(Pong)));
        m_fallBackUpdateTimer->stop();
    }
    m_source = source;
    if (m_source) {
        connect(m_source, SIGNAL(updateIntervalChanged(int)), this, SLOT(handleUpdateIntervalChanged(int)));
        connect(m_source, SIGNAL(pongReceived(Pong)), SLOT(handlePongReceived(Pong)));
        handleUpdateIntervalChanged(m_source->updateInterval());
        m_fallBackUpdateTimer->start();
    }

    m_canvasWidget->setSource(source);
    updateLabel();
}

void MonitorWidget::handlePongReceived(const Pong& pong)
{
    m_lastPong = pong;

    m_updateTimer->stop();
    m_fallBackUpdateTimer->start(); // restart
    updateLabel();
}

void MonitorWidget::handleUpdateIntervalChanged(int ms)
{
    const int threshold = (JUST_NOW_THRESHOLD + ms);
    m_fallBackUpdateTimer->setInterval(threshold);
}

void MonitorWidget::updateLabel()
{
    if (!source()) {
        m_label->setText(tr("Status: No monitor source connected!"));
        return;
    }

    const Pong& pong = m_lastPong;
    if (!m_lastPong.isValid()) {
        m_label->setText(tr("Status: No data available (yet)"));
    } else {
        const int elapsed = pong.time.elapsed();
        const int updateInterval = source()->updateInterval();
        const int threshold = (JUST_NOW_THRESHOLD + updateInterval);
        m_label->setText(tr("Last pong: <strong>%1</strong><br/>(with delay of <strong>%2</strong>)<br/>Update interval: <strong>%3</strong>")
            .arg(elapsed < threshold
                ? tr("&lt; %1s ago (within threshold)").arg(QString::number(threshold/1000))
                : tr("%1 ago").arg(formattedTime(elapsed, 0)))
            .arg(formattedTime(pong.delay))
            );
    }
}

#include "MonitorWidget.moc"
