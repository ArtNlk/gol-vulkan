#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

#include <limits>
#include <vulkan/vulkan.h>

#include <vector>
#include <expected>

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

    VkSwapchainKHR rawHandle();

    std::vector<VkImage> getImages() const;

    std::expected<uint32_t, VkResult> getNextImage(VkSemaphore semaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE,
                          uint64_t timeoutNs = std::numeric_limits<uint64_t>::max());

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
