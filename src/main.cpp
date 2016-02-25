#include "MainWindow.h"
#include "Utils.h"
#include "SourceFactory.h"

#include <QApplication>

void print_usage()
{
    printf("Usage: lag-monitor [OPTION] [HOST]"
"\n"
"Monitors the round-trip time via different mechanisms\n"
"By default, the monitor tries to measure round-trip times via pings to some configured host."
"\n"
"Options:\n"
"  -h, --help        Display this help\n"
"\n"
"  -s SOURCE   Select source, one of ['mock', 'ping']. Defaults to 'ping'\n"
"  -h HOST     Set target host for pings. Defaults to '8.8.8.8' (Google server)\n"
"\n"
"Report bugs to https://github.com/krf/lag-monitor\n"
"Copyright 2012 Kevin Funk <info@kfunk.org>\n"
    );
}

static const QStringList VALID_SOURCES = QStringList() << "ping" << "mock";

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // parse and evaluate argument list
    const QStringList args = app.arguments();

    // possible options
    bool printHelp = false;
    QString sourceId;
    QString host;
    QString invalidArgument; // for error reporting

    // evaluate
    QListIterator<QString> it(args);
    it.next(); // skip appname
    while (it.hasNext()) {
        const QString arg = it.next();
        if (arg == "--help" || arg == "-h") {
            printHelp = true;
            break; // done
        } else if (arg == "-s" && it.hasNext()) {
            sourceId = it.next();

            // validate parameter
            if (!VALID_SOURCES.contains(sourceId)) {
                invalidArgument = arg;
                break; // error
            }
        } else if (arg == "-h" && it.hasNext()) {
            host = it.next();
        } else {
            invalidArgument = arg;
            break; // error
        }
    }

    if (!invalidArgument.isEmpty()) {
        printf("Invalid option or missing/invalid parameter: %s\n", qPrintable(invalidArgument));
        printf("You probably need --help\n");
        return 1;
    }

    if (printHelp) {
        print_usage();
        return 0;
    }

    // proceed
    app.setWindowIcon(QIcon::fromTheme("view-statistics"));

    MainWindow window;
    if (!host.isEmpty())
        window.setHost(host);
    if (!sourceId.isEmpty()) {
        Source* source = SourceFactory::createSource(sourceId);
        Q_ASSERT(source);
        window.setSource(source);
    } else {
        window.setSource(new PingSource); // default
    }
    window.show();

    // activate
    window.startSource();

    return app.exec();
}
