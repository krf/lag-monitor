#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Source;
class MonitorWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow() override;

    void setHost(const QString& host);
    void setSource(Source* source);

public Q_SLOTS:
    void startSource();
    void stopSource();

private:
    MonitorWidget* m_monitorWidget = nullptr;
    Source* m_source = nullptr;
};

#endif // MAINWINDOW_H
