cmake_minimum_required(VERSION 3.16)

set(BIN_NAME dde-dock)

configure_file(environments.h.in environments.h @ONLY)

# Sources files
file(GLOB_RECURSE SRCS "*.h" "*.cpp" "../widgets/*.h" "../widgets/*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent DBus Svg Gui)
find_package(Dtk6Widget REQUIRED)
find_package(Dtk6CMake REQUIRED)

pkg_check_modules(XCB_EWMH REQUIRED xcb-ewmh x11)
pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)
pkg_check_modules(QGSettings REQUIRED gsettings-qt)
pkg_check_modules(DtkGUI REQUIRED dtkgui)

# driver-manager
add_executable(${BIN_NAME} ${SRCS} ${INTERFACES} item/item.qrc frame.qrc)
target_include_directories(${BIN_NAME} PUBLIC ${Dtk6Widget_INCLUDE_DIRS}
                                              ${XCB_EWMH_INCLUDE_DIRS}
                                              ${DFrameworkDBus_INCLUDE_DIRS}
                                              ${Qt6Gui_PRIVATE_INCLUDE_DIRS}
                                              ${PROJECT_BINARY_DIR}
                                              ${QGSettings_INCLUDE_DIRS}
                                              ${DtkGUI_INCLUDE_DIRS}
                                              ${Qt6Svg_INCLUDE_DIRS}
                                              ../interfaces)

target_link_libraries(${BIN_NAME} PRIVATE
    ${XCB_EWMH_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
    ${Dtk6Widget_LIBRARIES}
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::DBus
    ${QGSettings_LIBRARIES}
    ${DtkGUI_LIBRARIES}
    Qt6::Svg
)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "sw_64")
    target_compile_definitions(${BIN_NAME} PUBLIC DISABLE_SHOW_ANIMATION)
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "mips64")
    target_compile_definitions(${BIN_NAME} PUBLIC DISABLE_SHOW_ANIMATION)
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    target_compile_definitions(${BIN_NAME} PUBLIC DISABLE_SHOW_ANIMATION)
endif()

# bin
install(TARGETS ${BIN_NAME} DESTINATION bin)
