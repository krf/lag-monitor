add_executable(lag-monitor WIN32
    sources/Source.cpp
    sources/PingSource.cpp
    sources/MockSource.cpp

    MonitorWidget.cpp
    Utils.cpp
    CanvasWidget.cpp
    MainWindow.cpp

    main.cpp
)
target_compile_features(lag-monitor PRIVATE cxx_override)
target_link_libraries(lag-monitor
    Qt5::Widgets
)

install(TARGETS lag-monitor ${INSTALL_TARGETS_DEFAULT_ARGS})
