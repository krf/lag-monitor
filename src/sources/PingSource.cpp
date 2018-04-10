#include "PingSource.h"

#include "Utils.h"

#include <QString>
#include <QTime>
#include <QTimer>

namespace {

QString PING_COMMAND() { return QStringLiteral("ping"); }
QString DEFAULT_HOST() { return QStringLiteral("8.8.8.8"); }

const int RETRY_INTERVALS[] = {
    10, 10, 10, 15, 15, // after 60 secs
    30, 30, 60, 60, 180, 180, // after 600 secs
}; // seconds
const int MAX_RETRY_INTERVAL_STATE = sizeof(RETRY_INTERVALS) / sizeof(int);

}

PingSource::PingSource(QObject* parent)
    : Source(parent)
    , m_retryTimer(new QTimer(this))
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
    return m_process.state() == (QProcess::Running || QProcess::Starting);
}

QString PingSource::host() const
{
    static const auto overrideHostFromEnv = qgetenv("PING_SOURCE_HOST");
    if (!overrideHostFromEnv.isEmpty()) {
        return QString::fromLatin1(overrideHostFromEnv);
    }

    if (!overrideHost().isEmpty()) {
        return overrideHost();
    }

    return DEFAULT_HOST();
}

void PingSource::start()
{
    if (isActive())
        stop();

    QStringList args;
    args << QStringLiteral("-i") << QString::number(updateInterval()/1000);
    args << QStringLiteral("-n"); // numeric
    args << host();

    debug() << "Starting process:" << qPrintable(PING_COMMAND()) << qPrintable(args.join(QLatin1Char(' ')));
    m_process.start(PING_COMMAND(), args);
}

void PingSource::stop()
{
    m_process.kill();
    m_process.waitForFinished();
}

void PingSource::readStdout()
{
    while (!m_process.atEnd()) {
        const QByteArray line = m_process.readLine().trimmed();

        // Message: "PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data."
        if (line.startsWith("PING")) {
            // ping successfully started, reset the retry interval state to beginning
            m_retryIntervalState = 0;
        }
        else if (line.contains("bytes from")) {
            // Message: "64 bytes from 8.8.8.8: icmp_req=1 ttl=50 time=51.2 ms"
            const auto start = line.lastIndexOf('=') + 1;
            const auto timeValue = line.mid(start, line.size() - start - 3); // cut off " ms"
            bool ok;
            const float rtt = timeValue.toFloat(&ok);
            if (ok) {
                emit pongReceived(Pong(rtt));
            }
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
    if (errorCode == 0)
        return;

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

