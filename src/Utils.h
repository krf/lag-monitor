#ifndef UTILS_H
#define UTILS_H

#include <QDebug>

#include <iosfwd>

/// qDebug() convenience macros
#define debug() qDebug() << Q_FUNC_INFO

namespace Utils
{
};

#endif // UTILS_H
