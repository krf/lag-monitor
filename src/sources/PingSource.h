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
    ~PingSource() override;

    bool isActive() const override;

    /**
     * If override host is set, use that one, else use a fallback
     *
     * @see overrideHost
     */
    QString host() const;

public Q_SLOTS:
    void start() override;
    void stop() override;

private Q_SLOTS:
    void readStdout();
    void readStderr();

    void scheduleRetry();

    void handleFinished(int,QProcess::ExitStatus);

private:
    QProcess m_process;

    QTimer* m_retryTimer = nullptr;
    int m_retryIntervalState = 0;
};

#endif // PINGSOURCE_H
