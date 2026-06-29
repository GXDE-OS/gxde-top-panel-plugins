cmake_minimum_required(VERSION 3.16)

set(PLUGIN_NAME "home_monitor")

project(${PLUGIN_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRCS "*.h" "*.cpp")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Dtk6Widget REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(DdeDockInterface REQUIRED dde-dock)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")

add_library(${PLUGIN_NAME} SHARED ${SRCS} home_monitor.qrc)

set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ./)

target_include_directories(${PLUGIN_NAME} PUBLIC
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Dtk6Widget_INCLUDE_DIRS}
    ${DdeDockInterface_INCLUDE_DIRS}
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt6::Widgets
    ${Dtk6Widget_LIBRARIES}
    ${DdeDockInterface_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX "/usr")

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-dock/plugins)
