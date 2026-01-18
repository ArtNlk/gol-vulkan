#ifndef VULKANLOGICALDEVICE_H
#define VULKANLOGICALDEVICE_H

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace VkWrap
{

class VulkanLogicalDevice
{
public:
    VulkanLogicalDevice(VkPhysicalDevice physDev, std::vector<VkDeviceQueueCreateInfo> &queues,
                        VkPhysicalDeviceFeatures features,
                        std::vector<std::string> extensions,
                        std::vector<std::string>& validationLayers);
    ~VulkanLogicalDevice();

    VkQueue getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex = 0);

protected:
    VkDevice m_device;
};

}
#endif // VULKANLOGICALDEVICE_H
