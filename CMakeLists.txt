cmake_minimum_required(VERSION 3.16)
project(svgwatch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg)

# Add executable
add_executable(svgwatch main.cpp)

# Link Qt6 libraries
target_link_libraries(svgwatch Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg)
