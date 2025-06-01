cmake_minimum_required(VERSION 3.21)  # Для поддержки qt_add_executable

project(diplom_app VERSION 0.1 LANGUAGES CXX)

# ----------------------------------------
# --- Настройка vcpkg toolchain -----------
# ----------------------------------------

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(DEFAULT_VCPKG_ROOT "D:/programming/vcpkg")
    set(DEFAULT_TRIPLET "x64-windows")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(DEFAULT_VCPKG_ROOT "/home/tnovikov/study/vcpkg")
    set(DEFAULT_TRIPLET "x64-linux")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

if(DEFINED ENV{VCPKG_ROOT})
    message(STATUS "Using vcpkg from environment: $ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file" FORCE)
else()
    message(STATUS "Using default vcpkg path: ${DEFAULT_VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "${DEFAULT_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file" FORCE)
endif()

if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "${DEFAULT_TRIPLET}" CACHE STRING "Vcpkg target triplet")
endif()

message(STATUS "Using vcpkg triplet: ${VCPKG_TARGET_TRIPLET}")

if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(FATAL_ERROR "Vcpkg toolchain file not found at: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# ----------------------------------------
# --- Общие настройки проекта -------------
# ----------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Включаем дебаг поиска пакетов (при необходимости, можно закомментировать)
set(CMAKE_FIND_DEBUG_MODE TRUE)

# ----------------------------------------
# --- Поиск библиотек ---------------------
# ----------------------------------------

find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport SerialPort)

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)

# ----------------------------------------
# --- Источники проекта -------------------
# ----------------------------------------

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    lib/qcustomplot.cpp
    lib/qcustomplot.h

    datadisplayer.h
    testgenerator.h
    trilaterator.h
    coordinates.h
    avg_array.h
    comander.h
    SerialPortReader.h

    rolewindow.cpp
    rolewindow.h

    mongodb/mongoservice.cpp
    mongodb/mongoservice.h

    mongodb/violation_log/violation_log.cpp
    mongodb/violation_log/violation_log.h
    mongodb/violation_log/filterdialog.cpp
    mongodb/violation_log/filterdialog.h
    mongodb/violation_log/filterdialog.ui

    mongodb/test_database.js
    globals.h
)

# ----------------------------------------
# --- Создание исполняемого файла --------
# ----------------------------------------

qt_add_executable(diplom_app
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    mongodb/text_database_displayer.h
    mongodb/text_database_displayer.cpp
    mongodb/trackerdb.h
    mongodb/trackerdb.cpp
    mongodb/sector/sector.h
    mongodb/sector/sector.cpp
    mongodb/sector/sectorlistdialog.h mongodb/sector/sectorlistdialog.cpp mongodb/sector/sectorlistdialog.ui
    mongodb/object/object.h
    mongodb/object/object.cpp
    mongodb/object/objectlistdialog.h mongodb/object/objectlistdialog.cpp mongodb/object/objectlistdialog.ui
    mongodb/movement_rule/movement_rule.h
    mongodb/movement_rule/movement_rule.cpp
    mongodb/movement_rule/movementrulelistdialog.h mongodb/movement_rule/movementrulelistdialog.cpp mongodb/movement_rule/movementrulelistdialog.ui
)

# ----------------------------------------
# --- Подключение include директорий ------
# ----------------------------------------

target_include_directories(diplom_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    # ${CMAKE_CURRENT_SOURCE_DIR}/mongodb
    # ${CMAKE_CURRENT_SOURCE_DIR}/mongodb/violation_log
)

# ----------------------------------------
# --- Линковка с библиотеками --------------
# ----------------------------------------

target_link_libraries(diplom_app PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::SerialPort

    $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
    $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
)

# Вывод связанных библиотек для отладки
get_target_property(LINKED_LIBS diplom_app LINK_LIBRARIES)
message(STATUS "Linked libraries for diplom_app: ${LINKED_LIBS}")

# ----------------------------------------
# --- Финализация Qt проекта ---------------
# ----------------------------------------

qt_finalize_executable(diplom_app)
