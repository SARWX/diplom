cmake_minimum_required(VERSION 3.21)  # для поддержки qt_add_executable

# Поддержка vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "D:/programming/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

# Используем toolchain из переменной среды
if(DEFINED ENV{VCPKG_ROOT})
    message(STATUS "Using vcpkg from $ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "" FORCE)
else()
    message(FATAL_ERROR "VCPKG_ROOT is not defined")
endif()
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
# set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")

project(diplom_app VERSION 0.1 LANGUAGES CXX)

# include(D:/programming/vcpkg/scripts/buildsystems/vcpkg.cmake)

# Обязательные флаги для Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)  # Qt6 требует минимум C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Логгирование
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

message(STATUS "ENV{VCPKG_ROOT} = $ENV{VCPKG_ROOT}")
message(STATUS "ENV{PATH} = $ENV{PATH}")

# Qt 6
find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport SerialPort)

message(STATUS "CMAKE_TOOLCHAIN_FILE: -D ${CMAKE_TOOLCHAIN_FILE} ENV: $ENV{CMAKE_TOOLCHAIN_FILE}")
# set(CMAKE_FIND_DEBUG_MODE TRUE)
# set(ENV{VCPKG_TRACE_FIND_PACKAGE} "ON")


# set(mongocxx_DIR "D:/programming/vcpkg/installed/x64-windows/share/mongocxx")
# set(bsoncxx_DIR "D:/programming/vcpkg/installed/x64-windows/share/bsoncxx")
# MongoDB C++ драйвер
# set(BSONCXX_STATIC ON)
# set(MONGOCXX_STATIC ON)
set(CMAKE_FIND_DEBUG_MODE TRUE)

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)

# Исходные файлы
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

# Создание исполняемого файла
qt_add_executable(diplom_app
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    violation_log.h
)

# Линковка с Qt6 и MongoDB
target_link_libraries(diplom_app
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::PrintSupport
    PRIVATE Qt6::SerialPort
    # PRIVATE mongocxx::mongocxx
    # PRIVATE bsoncxx::bsoncxx
    # PRIVATE mongocxx::mongocxx_static
    # PRIVATE bsoncxx::bsoncxx_static
)
target_link_libraries(diplom_app PRIVATE $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)
target_link_libraries(diplom_app PRIVATE $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)
get_target_property(LINKED_LIBS diplom_app LINK_LIBRARIES)
message(STATUS "Linked libraries for diplom_app: ${LINKED_LIBS}")


# Метаданные для macOS (можно оставить — не мешает)
set_target_properties(diplom_app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_include_directories(diplom_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)


# Финализация (только для Qt6)
qt_finalize_executable(diplom_app)
