#include "Utils.h"

QString Utils::formattedTime(int ms, int precision)
{
    if (ms >= 1000)
        return QObject::tr("%1 secs").arg(QString::number(ms/1000.0, 'f', precision));
    else
        return QObject::tr("%1 ms").arg(QString::number(ms));
}
