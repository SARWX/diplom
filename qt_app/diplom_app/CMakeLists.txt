cmake_minimum_required(VERSION 3.5)

project(diplom_app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS SerialPort REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        lib/qcustomplot.h
        lib/qcustomplot.cpp
        datadisplayer.h
        testgenerator.h
        trilaterator.h
        coordinates.h
        avg_array.h
        comander.h
        SerialPortReader.h
        rolewindow.h
        rolewindow.cpp
        mongoservice.h
        mongoservice.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(diplom_app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET diplom_app APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(diplom_app SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(diplom_app
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(diplom_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(diplom_app PRIVATE Qt5::SerialPort)
target_link_libraries(${PROJECT_NAME}
    mongocxx::mongocxx
    bsoncxx::bsoncxx
)


set_target_properties(diplom_app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)



if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(diplom_app)
endif()
