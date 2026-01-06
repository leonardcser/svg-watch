cmake_minimum_required(VERSION 3.16)
project(svgwatch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg)

# Add executable
add_executable(svgwatch main.cpp)

# Enable Objective-C++ on macOS
if(APPLE)
    set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
endif()

# Link Qt6 libraries
target_link_libraries(svgwatch Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg)

# Link Cocoa on macOS
if(APPLE)
    target_link_libraries(svgwatch ${COCOA_LIBRARY})
endif()

# Install
install(TARGETS svgwatch DESTINATION bin)
