#include "MainWindow.h"
#include "Utils.h"
#include "sources/SourceFactory.h"

#include "lag-monitor_version.h"

#include <QCommandLineParser>
#include <QApplication>

namespace {

const QStringList VALID_SOURCES() { return {QStringLiteral("ping"), QStringLiteral("mock")}; }

}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("lag-monitor"));
    QCoreApplication::setApplicationVersion(QStringLiteral(LAG_MONITOR_VERSION_STRING));

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main",
        "Lag monitor for measuring RTT of ICMP packets\n"
        "\n"
        "Monitors the round-trip time via different mechanisms\n"
        "By default, the monitor tries to measure round-trip times via pings to some configured host.\n"
        "\n"
        "Report bugs to https://github.com/krf/lag-monitor\n"
        "Copyright 2018 Kevin Funk <kfunk@kde.org>"
    ));
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({
        {QStringLiteral("source")},
        QCoreApplication::translate("main", "Select source, one of 'mock', 'ping' (defaults to 'ping')"),
        QStringLiteral("SOURCE"),
        QStringLiteral("ping")
    });
    parser.addOption({
        {QStringLiteral("host")},
        QCoreApplication::translate("main", "Set target host for pings (defaults to Google servers)"),
        QStringLiteral("HOST"),
        QStringLiteral("8.8.8.8")
    });

    parser.process(app);

    // validate
    if (parser.isSet(QStringLiteral("source")) && !VALID_SOURCES().contains(parser.value(QStringLiteral("source")))) {
        parser.showHelp(1);
        Q_UNREACHABLE();
    }

    // proceed
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("view-statistics")));

    MainWindow window;
    if (parser.isSet(QStringLiteral("host")))
        window.setHost(parser.value(QStringLiteral("host")));
    if (parser.isSet(QStringLiteral("source"))) {
        Source* source = SourceFactory::createSource(parser.value(QStringLiteral("source")));
        qWarning() << "SOURCE" << source << parser.value(QStringLiteral("source"));
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
