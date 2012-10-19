#include "PingSource.h"

#include "Utils.h"

#include <QString>
#include <QTime>
#include <QTimer>

static const QString PING_COMMAND("ping");
static const int PING_INTERVAL = 5;
static const QString DEFAULT_HOST("8.8.8.8");
static const int PING_COMMAND_RETRY_INTERVAL = 30000; // 30 secs

static const int MAX_VALUE = 30000; // 30 secs

PingSource::PingSource(QObject* parent)
    : Source(parent)
    , m_retryTimer(new QTimer(this))
{
    connect(&m_process, SIGNAL(readyReadStandardOutput()), SLOT(readStdout()));
    connect(&m_process, SIGNAL(readyReadStandardError()), SLOT(readStderr()));
    connect(&m_process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(handleFinished(int,QProcess::ExitStatus)));

    m_retryTimer->setInterval(30000);
    connect(m_retryTimer, SIGNAL(timeout()), SLOT(start()));
}

bool PingSource::isActive() const
{
    return m_process.state() == QProcess::Running;
}

void PingSource::setActive(bool active)
{
    if (active)
        start();
    else
        stop();
}

QString PingSource::host() const
{
    if (m_host.isEmpty())
        return DEFAULT_HOST;
    return m_host;
}

void PingSource::setOverrideHost(const QString& host)
{
    m_host = host;
}

void PingSource::start()
{
    if (isActive())
        return;

    debug();

    QStringList args;
    args << "-i" << QString::number(PING_INTERVAL);
    args <<  host();
    m_process.start(PING_COMMAND, args);
}

void PingSource::stop()
{
    m_process.kill();
}

void PingSource::readStdout()
{
    while (!m_process.atEnd()) {
        const QString line = m_process.readLine().trimmed();

        // 64 bytes from bk-in-f94.1e100.net (173.194.69.94): icmp_req=1 ttl=50 time=66.0 ms
        if (line.contains("bytes from")) {
            const QString timeEntry = line.section(' ', 6, 6); // "time=N"
            const QString timeValue = timeEntry.split('=').last(); // "N"
            bool ok;
            const float rtt = timeValue.toFloat(&ok);
            if (ok)
                emit pongReceived(Pong(rtt));
        }
        debug() << line;
    }
}

void PingSource::readStderr()
{
    const QByteArray out = m_process.readAllStandardError().trimmed();
    debug() << out;
}

void PingSource::handleFinished(int errorCode, QProcess::ExitStatus status)
{
    debug() << errorCode << status;
    scheduleRetry();
}

void PingSource::scheduleRetry()
{
    debug() << "Retrying to start 'ping' in" << m_retryTimer->interval() << "seconds";
    m_retryTimer->start();
}

#include "PingSource.moc"
