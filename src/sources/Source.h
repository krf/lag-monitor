#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include <QTime>

class QDebug;

struct Pong
{
    Pong(QTime time_, int delay_)
        : time(time_), delay(delay_) {}

    explicit Pong(int delay_)
        : time(QTime::currentTime()), delay(delay_) {}

    /// Create an instance with invalid values
    Pong()
        : delay(-1) {}

    inline bool isValid() const {
        return time.isValid() && delay != -1;
    }

    /// time when this event occurred
    QTime time;
    /// delay in milliseconds
    int delay;
};

QDebug operator<<(QDebug dbg, const Pong& p);

/**
 * A source for catching pong events (abstract base class)
 *
 * See the subclasses for implementation details
 *
 * @note Sources are inactive by default, they need to be started first
 * @see start()
 */
class Source : public QObject
{
    Q_OBJECT

public:
    explicit Source(QObject* parent = 0);
    virtual ~Source();

    virtual bool isActive() const = 0;

    /// Set the update interval of this source in milliseconds
    void setUpdateInterval(int ms);
    /// Update interval in milliseconds
    int updateInterval() const { return m_updateInterval; }

    /// Return the host used to overwrite the default one
    QString overrideHost() const { return m_host; }
    /// Set the host used to overwrite the default one
    void setOverrideHost(const QString& host);

public Q_SLOTS:
    /// Set the source as active, async call
    virtual void start() = 0;
    /// Set the source as inactive, blocking call
    virtual void stop() = 0;

Q_SIGNALS:
    void pongReceived(const Pong& pong);

    void updateIntervalChanged(int ms);

protected:
    /**
     * React to update interval changes
     *
     * @note Default implementation just restarts the source
     */
    virtual void updateIntervalChangeEvent(int ms);

private:
    QString m_host;
    int m_updateInterval; // ms
};

#endif
