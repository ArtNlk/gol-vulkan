#include "VulkanLogicalDevice.h"

#include <stdexcept>

namespace VkWrap
{

VulkanLogicalDevice::VulkanLogicalDevice(VkPhysicalDevice physDev, std::vector<VkDeviceQueueCreateInfo> &queues,
                                         VkPhysicalDeviceFeatures features,
                                         std::vector<std::string> extensions,
                                         std::vector<std::string>& validationLayers)
{
    const bool validationLayersEnabled = !validationLayers.empty();

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = queues.data();
    createInfo.queueCreateInfoCount = queues.size();

    createInfo.pEnabledFeatures = &features;

    std::vector<const char*> cstrLayers(validationLayers.size());
    for(size_t i = 0; i < validationLayers.size(); i++)
    {
        cstrLayers[i] = validationLayers[i].c_str();
    }

    std::vector<const char*> cstrExtenstions(extensions.size(), nullptr);

    for(size_t i = 0; i < cstrExtenstions.size(); i++)
    {
        cstrExtenstions[i] = extensions[i].c_str();
    }

    createInfo.enabledExtensionCount = cstrExtenstions.size();
    createInfo.ppEnabledExtensionNames = cstrExtenstions.data();

    if (validationLayersEnabled)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(cstrLayers.size());
        createInfo.ppEnabledLayerNames = cstrLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physDev, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
}

VkDevice VulkanLogicalDevice::rawHandle()
{
    return m_device;
}

VkQueue VulkanLogicalDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex)
{
    VkQueue output = VK_NULL_HANDLE;

    vkGetDeviceQueue(m_device, queueFamilyIndex, queueIndex, &output);

    return output;
}

}
