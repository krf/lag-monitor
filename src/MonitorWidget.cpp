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

static QString formattedTime(int ms)
{
    if (ms > 1000)
        return QObject::tr("%1 secs").arg(QString::number(ms/1000.0, 'f', 2));
    else
        return QObject::tr("%1 ms").arg(QString::number(ms));
}

MonitorWidget::MonitorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_source(0)
    , m_fallBackUpdateTimer(new QTimer(this))
    , m_label(new QLabel)
    , m_canvasWidget(new CanvasWidget)
{
    m_label->setMargin(5);
    m_canvasWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(m_label);
    vbox->addWidget(m_canvasWidget);

    m_fallBackUpdateTimer->setInterval(5000);
    connect(m_fallBackUpdateTimer, SIGNAL(timeout()), SLOT(updateLabel()));

    updateLabel();
}

MonitorWidget::~MonitorWidget()
{
    debug();
}

void MonitorWidget::setSource(Source* source)
{
    if (m_source) {
        disconnect(m_source, SIGNAL(pongReceived(Pong)), this, SLOT(handlePongReceived(Pong)));
        m_fallBackUpdateTimer->stop();
    }
    m_source = source;
    if (m_source) {
        connect(m_source, SIGNAL(pongReceived(Pong)), SLOT(handlePongReceived(Pong)));
    }

    m_canvasWidget->setSource(source);
    updateLabel();
}

void MonitorWidget::handlePongReceived(const Pong& pong)
{
    m_lastPong = pong;
    m_fallBackUpdateTimer->start(); // restart
    updateLabel();
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
        m_label->setText(tr("Last pong received: <strong>%1</strong><br/>(with delay of <strong>%2</strong>)")
            .arg(elapsed < JUST_NOW_THRESHOLD
                ? tr("Just now (&lt; %1s)").arg(JUST_NOW_THRESHOLD/1000)
                : tr("%1 ago").arg(formattedTime(elapsed)))
            .arg(formattedTime(pong.delay)));
    }
}

#include "MonitorWidget.moc"
