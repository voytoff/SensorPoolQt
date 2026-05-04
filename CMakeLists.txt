cmake_minimum_required(VERSION 3.20)
project(SensorPool VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Quick
    Core
    Widgets
    LinguistTools
    Svg
    Concurrent
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_standard_project_setup()

set(RESOURCE_FILES res.qrc)
qt6_add_resources(PROJECT_RESOURCES ${RESOURCE_FILES})

qt_add_executable(appSensorPool
    main.cpp
    mainwindow.h mainwindow.cpp
    ${PROJECT_RESOURCES}
    sensorproperties.h sensorproperties.cpp sensorproperties.ui
    sensormodel.h sensormodel.cpp

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
