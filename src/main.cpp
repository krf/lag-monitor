#include "MainWindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("view-statistics"));

    MainWindow window;
    window.show();

    return app.exec();
}
