#include "VulkanPhysicalDevice.h"

namespace VkWrap
{

VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice device) :
    m_physDevice(device)
{

}

std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getQueues()
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

}
