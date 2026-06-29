
set(PLUGIN_NAME "datetime")

project(${PLUGIN_NAME})

# Sources files
file(GLOB SRCS "*.h" "*.cpp" "../../widgets/*.h" "../../widgets/*.cpp")

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg DBus)
find_package(Dtk6Widget REQUIRED)
find_package(Dtk6Core REQUIRED)

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN_NAME} SHARED ${SRCS} datetime.qrc)
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../)
target_include_directories(${PLUGIN_NAME} PUBLIC ../../interfaces)
target_link_libraries(${PLUGIN_NAME} PRIVATE
  Qt6::DBus
  Dtk6::Widget
  Dtk6::Core
  Qt6::Widgets
  Qt6::Svg
)

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-dock/plugins)
#install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/dde-top-panel/plugins)
#install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION lib/gxde-top-panel/plugins)
