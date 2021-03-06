#include "MainWindow.h"

#include "CanvasWidget.h"
#include "MonitorWidget.h"
#include "sources/PingSource.h"
#include "sources/MockSource.h"

MainWindow::MainWindow()
    : m_monitorWidget(new MonitorWidget)
{
    // override time span
    m_monitorWidget->canvas()->setTimeSpan(120000);

    setCentralWidget(m_monitorWidget);
    setWindowTitle(tr("Lag Monitor"));
}

void MainWindow::setHost(const QString& host)
{
    if (!m_source)
        return;

    m_source->setOverrideHost(host);
}

void MainWindow::setSource(Source* source)
{
    if (m_source == source)
        return;

    if (m_source) {
        m_monitorWidget->setSource(nullptr);
    }
    m_source = source;
    if (m_source) {
        m_monitorWidget->setSource(m_source);
    }
}

void MainWindow::startSource()
{
    if (!m_source)
        return;

    m_source->start();
}

void MainWindow::stopSource()
{
    if (!m_source)
        return;

    m_source->stop();
}

MainWindow::~MainWindow()
{}

