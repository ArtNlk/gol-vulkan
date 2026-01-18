find_package(Vulkan REQUIRED)

set(HEADERS VKWrap.h
    VulkanInstance.h
    VulkanDebugMessenger.h
)

set(SOURCES
    VulkanInstance.cpp
    VulkanDebugMessenger.cpp
)

add_library(VulkanWrapper ${SOURCES} ${HEADERS}
    VulkanPhysicalDevice.h VulkanPhysicalDevice.cpp
    VulkanLogicalDevice.h VulkanLogicalDevice.cpp
    VulkanSurface.h VulkanSurface.cpp
    VulkanQueue.h VulkanQueue.cpp
)

target_compile_options(VulkanWrapper PRIVATE
    -Wreturn-type
    -Werror=return-type
)

target_include_directories(VulkanWrapper PUBLIC "./")
target_include_directories(VulkanWrapper PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanWrapper Vulkan::Vulkan)
