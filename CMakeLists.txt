cmake_minimum_required(VERSION 3.14)

PROJECT("GOL-vulkan")
include(FetchContent)

set(LINUX_WAYLAND TRUE)

add_subdirectory("gol-app")
add_subdirectory("vulkan-wrapper")
