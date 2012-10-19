#include "Source.h"

#include "Utils.h"

Source::Source(QObject* parent)
    : QObject(parent)
    , m_updateInterval(5000)
{
}

Source::~Source()
{
    debug();
}

void Source::setUpdateInterval(int ms)
{
    if (m_updateInterval == ms)
        return;

    m_updateInterval = ms;
    updateIntervalChangeEvent(ms);
    emit updateIntervalChanged(ms);
}

void Source::updateIntervalChangeEvent(int ms)
{
    Q_UNUSED(ms);
    stop();
    start();
}

QDebug operator<<(QDebug dbg, const Pong& p)
{
    return dbg.nospace() << "Pong["
        << "time=" << qPrintable(p.time.toString("hh:mm:ss.zzz"))
        << ", delay=" << p.delay
        << "]";
    return dbg.space();
}

#include "Source.moc"