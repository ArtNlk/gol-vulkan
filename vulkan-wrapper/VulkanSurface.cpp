#include "VulkanSurface.h"

VkWrap::VulkanSurface::VulkanSurface(VkSurfaceKHR surface, VkInstance instance) :
    m_surface(surface),
    m_instance(instance)
{

}

VkWrap::VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

VkSurfaceKHR VkWrap::VulkanSurface::rawHandle()
{
    return m_surface;
}

VkWrap::VulkanSurface::SwapChainSupportDetails VkWrap::VulkanSurface::getDeviceSwapchainCapabilities(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}
