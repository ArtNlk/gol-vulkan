#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanQueue
{
public:
    VulkanQueue(VkQueue queue = VK_NULL_HANDLE);

protected:
    VkQueue m_queue;
};

}
#endif // VULKANQUEUE_H
