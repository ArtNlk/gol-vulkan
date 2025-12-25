find_package(Vulkan REQUIRED)

set(HEADERS VKWrap.h
    VulkanInstance.h)

set(SOURCES VulkanInstance.cpp)

add_library(VulkanWrapper ${SOURCES} ${HEADERS})

target_include_directories(VulkanWrapper PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanWrapper Vulkan::Vulkan)
