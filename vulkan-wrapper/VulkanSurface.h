#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VkWrap
{

class VulkanSurface
{
public:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    VulkanSurface(VkSurfaceKHR surface, VkInstance instance);
    ~VulkanSurface();

    VkSurfaceKHR rawHandle();

    SwapChainSupportDetails getDeviceSwapchainCapabilities(VkPhysicalDevice device);

protected:
    VkSurfaceKHR m_surface;
    VkInstance m_instance;
};

}
#endif // VULKANSURFACE_H
