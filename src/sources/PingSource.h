#ifndef PINGSOURCE_H
#define PINGSOURCE_H

#include "Source.h"

#include <QProcess>
#include <QTime>

class QTimer;

class PingSource : public Source
{
    Q_OBJECT

public:
    PingSource(QObject* parent = 0);
    virtual ~PingSource();

    virtual bool isActive() const;

    /**
     * If override host is set, use that one, else use a fallback
     *
     * @see overrideHost
     */
    QString host() const;

public Q_SLOTS:
    virtual void start();
    virtual void stop();

private Q_SLOTS:
    void readStdout();
    void readStderr();

    void scheduleRetry();

    void handleFinished(int,QProcess::ExitStatus);

private:
    QProcess m_process;

    QTimer* m_retryTimer;
    int m_retryIntervalState;
};

#endif // PINGSOURCE_H
