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

    virtual void setActive(bool active);
    virtual bool isActive() const;

    /**
     * If override host is set, use that one, else use a fallback
     *
     * @see overrideHost
     */
    QString host() const;

    QString overrideHost() const { return m_host; }
    void setOverrideHost(const QString& host);

public Q_SLOTS:
    void start();
    void stop();

private Q_SLOTS:
    void readStdout();
    void readStderr();

    void scheduleRetry();

    void handleFinished(int,QProcess::ExitStatus);

private:
    QProcess m_process;
    QString m_host;

    QTimer* m_retryTimer;
};

#endif // PINGSOURCE_H
