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
    if (name == "mock")
        return new MockSource;
    else if (name == "ping")
        return new PingSource;
    return 0;
}

#endif // SOURCEFACTORY_H
