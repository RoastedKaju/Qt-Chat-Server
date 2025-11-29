cmake_minimum_required(VERSION 3.19)
project(Client-Application LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Network)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(Client-Application
    WIN32 MACOSX_BUNDLE
    main.cpp
    main_window.cpp
    main_window.h
    main_window.ui
    dialog.h dialog.cpp dialog.ui
    client_manager.h client_manager.cpp

)

target_link_libraries(Client-Application
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)
target_link_libraries(Client-Application PRIVATE Qt6::Widgets)
target_link_libraries(Client-Application PRIVATE Qt6::Core)

include(GNUInstallDirs)

install(TARGETS Client-Application
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Client-Application
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
