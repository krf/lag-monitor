#include "MainWindow.h"
#include "Utils.h"

#include <QApplication>
#include <QDebug>

void print_usage()
{
    printf("Usage: lag-monitor [OPTION] [HOST]"
"\n"
"Monitors the round-trip time to HOST (defaults to 8.8.8.8)\n"
"\n"
"Options:\n"
"  -h, --help       Display this help\n"
"\n"
"Report bugs to https://github.com/krf/lag-monitor\n"
"Copyright 2012 Kevin Funk <info@kfunk.org>\n"
    );
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // parse and evaluate argument list
    const QStringList args = app.arguments();

    bool printHelp = false;
    QString host;
    for (int i = 1; i < args.size(); ++i) {
        const QString arg = args.at(i);
        if (arg == "--help" || arg == "-h") {
            printHelp = true; break; // done
        } else {
            host = arg; break; // done
        }
    }

    if (printHelp) {
        print_usage();
        return 1;
    }

    // proceed
    app.setWindowIcon(QIcon::fromTheme("view-statistics"));

    MainWindow window;
    window.setHost(host);
    window.show();

    return app.exec();
}
