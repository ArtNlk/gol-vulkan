#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VkWrap
{

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(VkPhysicalDevice device);

    VkPhysicalDevice rawHandle();

    std::vector<VkQueueFamilyProperties> getQueueProperties();

    std::vector<VkExtensionProperties> getExtensionProperties();

    VkPhysicalDeviceProperties getProperties();

    VkPhysicalDeviceFeatures getFeatures();

protected:
    VkPhysicalDevice m_physDevice;
};

}
#endif // VULKANPHYSICALDEVICE_H
