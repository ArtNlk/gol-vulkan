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


    std::vector<VkQueueFamilyProperties> getQueues();

protected:
    VkPhysicalDevice m_physDevice;
};

}
#endif // VULKANPHYSICALDEVICE_H
