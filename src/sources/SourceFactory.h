#ifndef SOURCEFACTORY_H
#define SOURCEFACTORY_H

#include "MockSource.h"
#include "PingSource.h"

#include <QString>

class SourceFactory
{
public:
    static Source* createSource(const QString& name);
};

Source* SourceFactory::createSource(const QString& name)
{
    if (name == QStringLiteral("mock"))
        return new MockSource;
    else if (name == QStringLiteral("ping"))
        return new PingSource;
    return nullptr;
}

#endif // SOURCEFACTORY_H
