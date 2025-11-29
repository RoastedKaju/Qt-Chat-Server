cmake_minimum_required(VERSION 3.19)
project(Qt-Chat-Server LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(Qt-Chat-Server
    main.cpp
    server_manager.h server_manager.cpp
)

target_link_libraries(Qt-Chat-Server
    PRIVATE
        Qt::Core
        Qt::Network
)
target_link_libraries(Qt-Chat-Server PRIVATE Qt6::Core)

include(GNUInstallDirs)

install(TARGETS Qt-Chat-Server
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
