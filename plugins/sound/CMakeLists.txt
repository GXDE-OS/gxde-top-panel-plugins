
set(PLUGIN_NAME "sound")

project(${PLUGIN_NAME})

# Sources files
file(GLOB_RECURSE SRCS "*.h" "*.cpp" "../../widgets/*.h" "../../widgets/*.cpp" "../../frame/util/imageutil.h" "../../frame/util/imageutil.cpp")

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg DBus)
find_package(Dtk6Widget REQUIRED)
find_package(Dtk6Core REQUIRED)
find_package(DFrameworkdbusQt6 CONFIG REQUIRED)

pkg_check_modules(QGSettings REQUIRED gsettings-qt6)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN_NAME} SHARED ${SRCS} resources/sound.qrc)
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../system-trays)
target_include_directories(${PLUGIN_NAME} PUBLIC
  ${QGSettings_INCLUDE_DIRS}
  ../../interfaces
  ../../frame)
target_link_libraries(${PLUGIN_NAME} PRIVATE
  Dtk6::Widget
  Dtk6::Core
  DFrameworkdbusQt6::DFrameworkdbusQt6
  ${QGSettings_LIBRARIES}
  Qt6::DBus
  Qt6::Widgets
  Qt6::Svg
)

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-dock/plugins/system-trays)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-top-panel/plugins)
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/gxde-top-panel/plugins)
