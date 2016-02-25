include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}/sources

    ${QT_INCLUDES}
)

set(qtapp_SRCS
    sources/Source.cpp
    sources/PingSource.cpp
    sources/MockSource.cpp

    MonitorWidget.cpp
    Utils.cpp
    CanvasWidget.cpp
    MainWindow.cpp
    main.cpp
)
add_executable(lag-monitor ${qtapp_SRCS})
target_link_libraries(lag-monitor
    Qt5::Widgets
)

install(TARGETS lag-monitor ${INSTALL_TARGETS_DEFAULT_ARGS})
