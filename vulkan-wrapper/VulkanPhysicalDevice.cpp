#include "VulkanPhysicalDevice.h"

namespace VkWrap
{

VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice device) :
    m_physDevice(device)
{

}

VkPhysicalDevice VulkanPhysicalDevice::rawHandle()
{
    return m_physDevice;
}

std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getQueueProperties()
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &queueFamilyCount, nullptr);

    if(queueFamilyCount == 0)
    {
        return std::vector<VkQueueFamilyProperties>();
    }

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physDevice, &queueFamilyCount, queueFamilies.data());

    return queueFamilies;
}

std::vector<VkExtensionProperties> VulkanPhysicalDevice::getExtensionProperties()
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(m_physDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(m_physDevice, nullptr, &extensionCount, availableExtensions.data());

    return availableExtensions;
}

VkPhysicalDeviceProperties VulkanPhysicalDevice::getProperties()
{
    VkPhysicalDeviceProperties deviceProperties;

    vkGetPhysicalDeviceProperties(m_physDevice, &deviceProperties);

    return deviceProperties;
}

VkPhysicalDeviceFeatures VulkanPhysicalDevice::getFeatures()
{
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(m_physDevice, &deviceFeatures);

    return deviceFeatures;
}

}
