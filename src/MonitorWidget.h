#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>

#include "sources/Source.h"

class QLabel;
class QTimer;

class CanvasWidget;

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
    ~MonitorWidget() override;

    void setSource(Source* source);
    Source* source() const { return m_source; }

    /**
     * Retrieve the canvas widget for this monitor
     */
    CanvasWidget* canvas() const { return m_canvasWidget; }

private Q_SLOTS:
    void handlePongReceived(const Pong& pong);
    void handleUpdateIntervalChanged(int ms);

    /// regularly called to update the label
    void updateLabel();

private:
    Source* m_source = nullptr;
    QTimer* m_fallBackUpdateTimer = nullptr;
    QTimer* m_updateTimer = nullptr;

    // ui
    QLabel* m_label = nullptr;
    CanvasWidget* m_canvasWidget = nullptr;

    Pong m_lastPong;
};

#endif // MONITORWIDGET_H
