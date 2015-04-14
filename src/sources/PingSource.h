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
    PingSource(QObject* parent = nullptr);
    virtual ~PingSource();

    virtual bool isActive() const override;

    /**
     * If override host is set, use that one, else use a fallback
     *
     * @see overrideHost
     */
    QString host() const;

public Q_SLOTS:
    virtual void start() override;
    virtual void stop() override;

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
