#include "MonitorWidget.h"

#include "CanvasWidget.h"
#include "sources/Source.h"
#include "Utils.h"

#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QTimer>

static const int JUST_NOW_THRESHOLD = 1000; // ms

MonitorWidget::MonitorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_updateTimer(new QTimer(this))
    , m_label(new QLabel)
    , m_canvasWidget(new CanvasWidget)
{
    m_label->setMargin(5);
    m_canvasWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

    auto vbox = new QVBoxLayout(this);
    vbox->addWidget(m_label);
    vbox->addWidget(m_canvasWidget);

    m_updateTimer->setInterval(1000);
    m_updateTimer->start();
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(updateLabel()));

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
    }
    m_source = source;
    if (m_source) {
        connect(m_source, SIGNAL(updateIntervalChanged(int)), this, SLOT(handleUpdateIntervalChanged(int)));
        connect(m_source, SIGNAL(pongReceived(Pong)), SLOT(handlePongReceived(Pong)));
        handleUpdateIntervalChanged(m_source->updateInterval());
    }

    m_canvasWidget->setSource(source);
    updateLabel();
}

void MonitorWidget::handlePongReceived(const Pong& pong)
{
    m_lastPong = pong;
}

void MonitorWidget::handleUpdateIntervalChanged(int updateInterval)
{
    setToolTip(tr("<nobr>Update interval: <strong>%3</strong></nobr>")
        .arg(Utils::formattedTime(updateInterval)));
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
        m_label->setText(tr("Pong: <strong>%1</strong> | \u0394: <strong>%2</strong>")
            .arg(elapsed < threshold
                ? tr("OK")
                : tr("<strong>%1%</strong> ago").arg(Utils::formattedTime(elapsed)))
            .arg(Utils::formattedTime(pong.delay, 2)));
        m_label->setToolTip(tr("Last pong received %1 ago (threshold is %2), pong was delayed %3")
            .arg(Utils::formattedTime(elapsed))
            .arg(Utils::formattedTime(threshold))
            .arg(Utils::formattedTime(pong.delay, 2))
        );
    }
}

