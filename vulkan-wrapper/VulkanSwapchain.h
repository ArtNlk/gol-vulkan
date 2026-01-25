#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VkWrap
{

class VulkanSwapchain
{
public:
    VulkanSwapchain(VkDevice device, VkSurfaceKHR surface,
                    VkSurfaceFormatKHR format,
                    VkPresentModeKHR presentMode,
                    VkSurfaceCapabilitiesKHR capabilities,
                    VkExtent2D extent,
                    uint32_t imageCount,
                    std::vector<uint32_t> queueFamilyIdxs);

    ~VulkanSwapchain();

    std::vector<VkImage> getImages() const;

    VkFormat format() const;
    VkExtent2D extent() const;

protected:
    VkSwapchainKHR m_swapchain;
    VkDevice m_device;
    VkFormat m_format;
    VkExtent2D m_extent;
};

}
#endif // VULKANSWAPCHAIN_H
