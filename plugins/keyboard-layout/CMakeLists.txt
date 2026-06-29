
set(PLUGIN_NAME "keyboard-layout")

project(${PLUGIN_NAME})

# Sources files
file(GLOB_RECURSE SRCS "*.h" "*.cpp")

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg DBus)
find_package(Dtk6Widget REQUIRED)

pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)
pkg_check_modules(QGSettings REQUIRED gsettings-qt)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN_NAME} SHARED ${SRCS})
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../)
target_include_directories(${PLUGIN_NAME} PUBLIC ${DFrameworkDBus_INCLUDE_DIRS}
                                                 ${QGSettings_INCLUDE_DIRS}
                                                 ../../interfaces)
target_link_libraries(${PLUGIN_NAME} PRIVATE
    Dtk6::Widget
    ${DFrameworkDBus_LIBRARIES}
    ${QGSettings_LIBRARIES}
    Qt6::Widgets
    Qt6::Svg
    Qt6::DBus
)

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-dock/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-top-panel/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/gxde-top-panel/plugins)
install(FILES ./keybord_layout.json DESTINATION /etc/dde-dock/indicator)
