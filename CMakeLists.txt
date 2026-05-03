cmake_minimum_required(VERSION 3.14)

PROJECT("GOL-vulkan")
include(FetchContent)

FetchContent_Declare(
    VulkanMemoryAllocator
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG v3.1.0
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(VulkanMemoryAllocator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINUX_WAYLAND TRUE)

add_subdirectory("gol-app")
add_subdirectory("vulkan-wrapper")
