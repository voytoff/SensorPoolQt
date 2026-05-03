cmake_minimum_required(VERSION 3.16)

project(SensorPool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(appSensorPool_resource_files
    res.qrc
)
qt_add_executable(appSensorPool
    main.cpp
    mainwindow.h mainwindow.cpp
    ${appSensorPool_resource_files}
)

set_target_properties(appSensorPool PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSensorPool
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSensorPool PRIVATE Qt6::Widgets Qt6::Quick)

include(GNUInstallDirs)
install(TARGETS appSensorPool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
