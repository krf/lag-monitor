#include "Source.h"

#include "Utils.h"

Source::Source(QObject* parent)
    : QObject(parent)
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
    if (isActive()) {
        stop();
        start();
    }
}

void Source::setOverrideHost(const QString& host)
{
    if (m_host == host)
        return;

    m_host = host;
    if (isActive())
        start();
}

QDebug operator<<(QDebug dbg, const Pong& p)
{
    return dbg.nospace() << "Pong["
        << "time=" << qPrintable(p.time.toString(QStringLiteral("hh:mm:ss.zzz")))
        << ", delay=" << p.delay
        << "]";
    return dbg.space();
}

