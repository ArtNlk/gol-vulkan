#ifndef VULKANCOMMANDPOOL_H
#define VULKANCOMMANDPOOL_H

#include <vulkan/vulkan.h>

#include "VulkanCommandBuffer.h"

namespace VkWrap
{

class VulkanCommandPool
{
public:
    VulkanCommandPool(VkDevice device, uint32_t queueFamilyIndex);
    ~VulkanCommandPool();

    VulkanCommandBuffer createCommandBuffer();

protected:
    VkCommandPool m_pool;
    VkDevice m_device;
};

}
#endif // VULKANCOMMANDPOOL_H
