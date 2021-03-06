#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QFrame>
#include <QQueue>

class QTimer;
class Pong;
class Source;

class CanvasWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);

    void setSource(Source* source);
    Source* source() const { return m_source; }

    int timeSpan() const { return m_timeSpan; }
    /**
     * Set the timespan this widget should display
     *
     * @param timeSpan Value in milliseconds for setting the time span
     * The widget will display all events that have occurred in the range [now, timeSpan]
     *
     * @note Minimum for timeSpan is 5000
     */
    void setTimeSpan(int timeSpan);

    QSize minimumSizeHint() const override;

private Q_SLOTS:
    void handlePongReceived(const Pong& pong);

protected:
    void paintEvent(QPaintEvent* e) override;

private:
    void paintForeground(QPainter* painter) const;
    void paintBackground(QPainter* painter) const;

    Source* m_source = nullptr;
    QTimer* m_updateTimer = nullptr;

    /// FIFO, head -> oldest entry, tail -> newest entry
    QQueue<Pong> m_history;
    int m_timeSpan = 60000; // ms

    // cache
    int m_maxDelay = -1;
};

#endif // CANVASWIDGET_H
