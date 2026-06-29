
set(PLUGIN_NAME "disk-mount")

project(${PLUGIN_NAME})

# Sources files
file(GLOB SRCS "*.h" "*.cpp")

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg DBus)
find_package(Dtk6Widget REQUIRED)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN_NAME} SHARED ${SRCS} resources.qrc)
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../)
target_include_directories(${PLUGIN_NAME} PUBLIC ../../interfaces)
target_link_libraries(${PLUGIN_NAME} PRIVATE
    Dtk6::Widget
    Qt6::Widgets
    Qt6::Svg
    Qt6::DBus
)

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-dock/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-top-panel/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/gxde-top-panel/plugins)
