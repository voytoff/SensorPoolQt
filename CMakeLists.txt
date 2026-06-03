cmake_minimum_required(VERSION 3.20)
project(SensorPull VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Quick
    Core
    Widgets
    LinguistTools
    Svg
    Gui
    Concurrent
    WebSockets
)

qt_standard_project_setup()

set(RESOURCE_FILES res.qrc)
qt6_add_resources(PROJECT_RESOURCES ${RESOURCE_FILES})

qt_add_executable(SensorPull
    main.cpp
    mainwindow.h mainwindow.cpp
    ${PROJECT_RESOURCES}
    sensorproperties.h sensorproperties.cpp sensorproperties.ui
    sensormodel.h sensormodel.cpp
    sensorport.h sensorport.cpp
    sensor.h sensor.cpp
    settings.cpp settings.h
    settingsdlg.h settingsdlg.cpp settingsdlg.ui
    closablewidget.h closablewidget.cpp
    sensorsettings.h sensorsettings.cpp sensorsettings.ui
)

set(GLOBALLIBDIR "../GlobalLibQt")
include_directories(${GLOBALLIBDIR})
if (NOT TARGET GlobalLib)
    add_subdirectory(${GLOBALLIBDIR} GlobalLib)
endif()

set(MMPFILEDIR "../MMPFileQt")
include_directories(${MMPFILEDIR}/lib)
if (NOT TARGET MMPFile)
    add_subdirectory(${MMPFILEDIR} MMPFile)
endif()

set(SCHEMEHELPEREDIR "../SchemeHelperQt")
include_directories(${SCHEMEHELPEREDIR})
add_subdirectory(${SCHEMEHELPEREDIR} SchemeHelper)

set_target_properties(SensorPull PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SensorPull
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(SensorPull PRIVATE
    Qt6::Widgets
    Qt6::Quick
    Qt6::Gui
    Qt6::WebSockets
    GlobalLib
    MMPFile
    SchemeHelper
)

include(GNUInstallDirs)
install(TARGETS SensorPull
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
