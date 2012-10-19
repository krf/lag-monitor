#include "MainWindow.h"

#include "CanvasWidget.h"
#include "MonitorWidget.h"
#include "PingSource.h"
#include "MockSource.h"

MainWindow::MainWindow()
{
    Source* source = new PingSource(this);

    MonitorWidget* monitorWidget = new MonitorWidget;
    monitorWidget->setSource(source);

    // override time span
    monitorWidget->canvas()->setTimeSpan(120000);

    setCentralWidget(monitorWidget);

    source->start();

    setWindowTitle(tr("Lag monitor"));
}

MainWindow::~MainWindow()
{}

#include "MainWindow.moc"
