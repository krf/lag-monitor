#ifndef MOCKSOURCE_H
#define MOCKSOURCE_H

#include "Source.h"

class QTimer;

class MockSource : public Source
{
    Q_OBJECT

public:
    MockSource(QObject* parent = nullptr);

    virtual bool isActive() const;

public Q_SLOTS:
    virtual void start();
    virtual void stop();

private Q_SLOTS:
    void generateValues();

private:
    QTimer* m_updateTimer;
};

#endif // MOCKSOURCE_H
