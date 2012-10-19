#include "MainWindow.h"

#include "CanvasWidget.h"
#include "MonitorWidget.h"
#include "PingSource.h"
#include "MockSource.h"

MainWindow::MainWindow()
    : m_monitorWidget(new MonitorWidget)
    , m_pingSource(new PingSource(this))
{
    m_monitorWidget->setSource(m_pingSource);

    // override time span
    m_monitorWidget->canvas()->setTimeSpan(120000);

    setCentralWidget(m_monitorWidget);

    m_pingSource->start();

    setWindowTitle(tr("Lag monitor"));
}

void MainWindow::setHost(const QString& host)
{
    m_pingSource->setOverrideHost(host);
}

MainWindow::~MainWindow()
{}

#include "MainWindow.moc"
