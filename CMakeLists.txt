cmake_minimum_required(VERSION 3.14)

PROJECT("GOL-vulkan")
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINUX_WAYLAND TRUE)

add_subdirectory("gol-app")
add_subdirectory("vulkan-wrapper")
