#include "MockSource.h"

#include <QTime>
#include <qglobal.h>
#include <QTimer>

static const int MAX_DELAY = 1000;
static const int MIN_DELAY = 100;

static const int MIN_TIMEDELTA = 5000; // ms
static const int MAX_TIMEDELTA = 10000; // ms

MockSource::MockSource(QObject* parent)
    : Source(parent)
    , m_updateTimer(new QTimer(this))
{
    qsrand(QTime::currentTime().elapsed());

    m_updateTimer->setInterval(MAX_TIMEDELTA);
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(generateValues()));
}

void MockSource::generateValues()
{
    const int randomInterval = qrand() % (MAX_TIMEDELTA - MIN_TIMEDELTA) + MIN_TIMEDELTA;
    m_updateTimer->setInterval(randomInterval);

    const int randomValue = qrand() % (MAX_DELAY - MIN_DELAY) + MIN_DELAY;
    emit pongReceived(Pong(randomValue));
}

void MockSource::start()
{
    m_updateTimer->start();
}

void MockSource::stop()
{
    m_updateTimer->stop();
}

bool MockSource::isActive() const
{
    return m_updateTimer->isActive();
}


#include "MockSource.moc"
