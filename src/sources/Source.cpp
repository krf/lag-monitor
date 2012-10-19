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

QDebug operator<<(QDebug dbg, const Pong& p)
{
    return dbg.nospace() << "Pong["
        << "time=" << qPrintable(p.time.toString("hh:mm:ss.zzz"))
        << ", delay=" << p.delay
        << "]";
    return dbg.space();
}

#include "Source.moc"