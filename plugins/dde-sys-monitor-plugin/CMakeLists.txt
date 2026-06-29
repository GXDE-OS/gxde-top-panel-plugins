cmake_minimum_required(VERSION 3.16)

set(PLUGIN_NAME "sys_monitor")

project(${PLUGIN_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRCS "*.h" "*.cpp" "*.ui")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Dtk6Widget REQUIRED)
find_package(Dtk6Gui REQUIRED)
find_package(PkgConfig REQUIRED)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")

add_library(${PLUGIN_NAME} SHARED ${SRCS} sys_monitor.qrc)

set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ./)
target_include_directories(${PLUGIN_NAME} PUBLIC ../../interfaces)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt6::Widgets
    Dtk6::Widget
    Dtk6::Gui
)

set(CMAKE_INSTALL_PREFIX "/usr")

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-top-panel/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/gxde-top-panel/plugins)
