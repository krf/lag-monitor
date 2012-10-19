#include "PingSource.h"

#include "Utils.h"

#include <QString>
#include <QTime>
#include <QTimer>

static const QString PING_COMMAND("ping");
static const QString DEFAULT_HOST("8.8.8.8");

static const int RETRY_INTERVALS[] = {
    10, 10, 10, 15, 15, // after 60 secs
    30, 30, 60, 60, 180, 180, // after 600 secs
}; // seconds
static const int MAX_RETRY_INTERVAL_STATE = sizeof(RETRY_INTERVALS) / sizeof(int);

PingSource::PingSource(QObject* parent)
    : Source(parent)
    , m_retryTimer(new QTimer(this))
    , m_retryIntervalState(0)
{
    connect(&m_process, SIGNAL(readyReadStandardOutput()), SLOT(readStdout()));
    connect(&m_process, SIGNAL(readyReadStandardError()), SLOT(readStderr()));
    connect(&m_process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(handleFinished(int,QProcess::ExitStatus)));

    m_retryTimer->setInterval(RETRY_INTERVALS[m_retryIntervalState] * 1000);
    m_retryTimer->setSingleShot(true);
    connect(m_retryTimer, SIGNAL(timeout()), SLOT(start()));
}

PingSource::~PingSource()
{
    stop();
}

bool PingSource::isActive() const
{
    return m_process.state() == QProcess::Running;
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

    QStringList args;
    args << "-i" << QString::number(updateInterval()/1000);
    args <<  host();

    debug() << "Starting process:" << qPrintable(PING_COMMAND) << qPrintable(args.join(" "));
    m_process.start(PING_COMMAND, args);
}

void PingSource::stop()
{
    m_process.kill();
    m_process.waitForFinished();
}

void PingSource::readStdout()
{
    while (!m_process.atEnd()) {
        const QString line = m_process.readLine().trimmed();

        // Message: "PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data."
        if (line.startsWith("PING")) {
            // ping successfully started, reset the retry interval state to beginning
            m_retryIntervalState = 0;
        }
        // Message: "64 bytes from 8.8.8.8: icmp_req=1 ttl=50 time=51.2 ms"
        else if (line.contains("bytes from")) {
            const QString timeEntry = line.section(' ', 6, 6); // "time=N"
            const QString timeValue = timeEntry.split('=').last(); // "N"
            bool ok;
            const float rtt = timeValue.toFloat(&ok);
            if (ok)
                emit pongReceived(Pong(rtt));
        }
        debug() << "-stdout-" << line;
    }
}

void PingSource::readStderr()
{
    const QByteArray out = m_process.readAllStandardError().trimmed();
    debug() << "-stderr-" << out;
}

void PingSource::handleFinished(int errorCode, QProcess::ExitStatus status)
{
    debug() << errorCode << status;
    scheduleRetry();
}

void PingSource::scheduleRetry()
{
    m_retryTimer->setInterval(RETRY_INTERVALS[m_retryIntervalState] * 1000);
    debug() << "Retrying to start 'ping' in" << m_retryTimer->interval()/1000 << "seconds";
    m_retryTimer->start();

    // go to next retry state
    m_retryIntervalState = qMin(MAX_RETRY_INTERVAL_STATE, m_retryIntervalState+1);
}

#include "PingSource.moc"
