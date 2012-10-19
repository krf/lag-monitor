#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class PingSource;
class MonitorWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();

    void setHost(const QString& host);

private:
    MonitorWidget* m_monitorWidget;
    PingSource* m_pingSource;
};

#endif // MAINWINDOW_H
