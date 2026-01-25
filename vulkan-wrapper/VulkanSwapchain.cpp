#include "VulkanSwapchain.h"

#include <stdexcept>

namespace VkWrap
{

VkWrap::VulkanSwapchain::VulkanSwapchain(VkDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR format,
                                         VkPresentModeKHR presentMode, VkSurfaceCapabilitiesKHR capabilities,
                                         VkExtent2D extent,
                                         uint32_t imageCount,
                                         std::vector<uint32_t> queueFamilyIdxs):
    m_device(device),
    m_format(format.format),
    m_extent(extent)
{
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (queueFamilyIdxs.size() > 1) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = queueFamilyIdxs.size();
        createInfo.pQueueFamilyIndices = queueFamilyIdxs.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = capabilities.currentTransform;

    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}

std::vector<VkImage> VulkanSwapchain::getImages() const
{
    std::vector<VkImage> output;
    uint32_t imageCount = 0;

    vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr);
    output.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, output.data());

    return output;
}

VkFormat VulkanSwapchain::format() const
{
    return m_format;
}

VkExtent2D VulkanSwapchain::extent() const
{
    return m_extent;
}

}
