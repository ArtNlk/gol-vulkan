#ifndef VULKANFENCE_H
#define VULKANFENCE_H

#include <limits>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace VkWrap
{

class VulkanFence
{
public:
    VulkanFence(VkDevice device, bool createSignaled = false);
    ~VulkanFence();

    VkFence rawHandle();

    void wait(uint64_t timeout = std::numeric_limits<uint64_t>::max());

    void resetFence();

protected:
    VkFence m_fence;
    VkDevice m_device;
};

}
#endif // VULKANFENCE_H
