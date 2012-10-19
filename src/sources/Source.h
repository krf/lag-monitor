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

class Source : public QObject
{
    Q_OBJECT

public:
    explicit Source(QObject* parent = 0);
    virtual ~Source();

    virtual void setActive(bool active) = 0;
    virtual bool isActive() const = 0;

    void activate() { setActive(true); }
    void deactivate() { setActive(false); }

Q_SIGNALS:
    void pongReceived(const Pong& pong);
};

#endif
