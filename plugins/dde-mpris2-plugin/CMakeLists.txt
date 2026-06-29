cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

set(PLUGIN_NAME "dde_mpris2")

project(${PLUGIN_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SRCS "*.h" "*.cpp" "*.qrc" "widgets/*.h" "widgets/*.cpp" "dbus/*.h" "dbus/*.cpp")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Network LinguistTools)
find_package(Dtk6Widget REQUIRED)
find_package(Dtk6Gui REQUIRED)
find_package(Dtk6Core REQUIRED)
find_package(PkgConfig REQUIRED)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")

add_library(${PLUGIN_NAME} SHARED ${SRCS} ${UIS_HDRS} ${QM_FILES} dbus/DBusMonitor.cpp dbus/DBusMonitor.h dbus/dbusinterface.h lyric/QQLyricFetcher.cpp lyric/QQLyricFetcher.h lyric/AbstractLyricFetcher.h lyric/MLyric.cpp lyric/MLyric.h)

set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ./)

target_include_directories(${PLUGIN_NAME} PUBLIC ../../interfaces)

target_compile_definitions(${PLUGIN_NAME} PUBLIC
        QT_DEBUG_NO_OUTPUT
        )

target_link_libraries(${PLUGIN_NAME} PRIVATE
        Qt6::Widgets
        Dtk6::Widget
        Dtk6::Gui
        Dtk6::Core
        Qt6::Network
        Qt6::DBus
        )

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-top-panel/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/gxde-top-panel/plugins)
