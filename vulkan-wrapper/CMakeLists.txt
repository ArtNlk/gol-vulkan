find_package(Vulkan REQUIRED)

set(HEADERS VKWrap.h
    VulkanInstance.h
    VulkanDebugMessenger.h
    VulkanPhysicalDevice.h
    VulkanLogicalDevice.h
    VulkanSurface.h
    VulkanQueue.h
    VulkanSwapchain.h
    VulkanImageView.h
    VulkanShaderModule.h
    VulkanPipeline.h
    VulkanPipelineLayout.h
    VulkanRenderPass.h
)

set(SOURCES
    VulkanInstance.cpp
    VulkanDebugMessenger.cpp
    VulkanPhysicalDevice.cpp
    VulkanLogicalDevice.cpp
    VulkanSurface.cpp
    VulkanQueue.cpp
    VulkanSwapchain.cpp
    VulkanImageView.cpp
    VulkanShaderModule.cpp
    VulkanPipeline.cpp
    VulkanPipelineLayout.cpp
    VulkanRenderPass.cpp
)

add_library(VulkanWrapper ${SOURCES} ${HEADERS})

target_compile_options(VulkanWrapper PRIVATE
    -Wreturn-type
    -Werror=return-type
)

target_include_directories(VulkanWrapper PUBLIC "./")
target_include_directories(VulkanWrapper PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanWrapper Vulkan::Vulkan)
