#ifndef UTILS_H
#define UTILS_H

#include <QDebug>

#include <iosfwd>

/// qDebug() convenience macros
#define debug() qDebug() << Q_FUNC_INFO

namespace Utils
{
    /**
     * Return a human-readable representation of the time in the argument list
     *
     * @example
     * - ms == 100 => "100 ms"
     * - ms == 1000 => "1 secs" if precision == 0
     * - ms == 1000 => "1.00 secs" if precision == 2
     * 
     * @param ms Time in milliseconds
     * @param precision Precision of the resulting double
     */
    QString formattedTime(int ms, int precision = 0);
};

#endif // UTILS_H
