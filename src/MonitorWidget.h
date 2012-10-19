#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>

#include "Source.h"

class QLabel;
class QTimer;

class CanvasWidget;

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~MonitorWidget();

    void setSource(Source* source);
    Source* source() const { return m_source; }

    /**
     * Retrieve the canvas widget for this monitor
     */
    CanvasWidget* canvas() const { return m_canvasWidget; }

private Q_SLOTS:
    void handlePongReceived(const Pong& pong);
    void handleUpdateIntervalChanged(int ms);

    /**
     * This function should be mainly called when new data points are received
     *
     * The fallback uptimer triggers this function in case no new data points
     * arrive for a certain amount of time
     *
     * @see handlePongReceived
     */
    void updateLabel();

private:
    Source* m_source;
    QTimer* m_fallBackUpdateTimer;
    QTimer* m_updateTimer;

    // ui
    QLabel* m_label;
    CanvasWidget* m_canvasWidget;

    Pong m_lastPong;
};

#endif // MONITORWIDGET_H
