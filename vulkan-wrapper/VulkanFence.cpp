#include "VulkanFence.h"

#include <stdexcept>

namespace VkWrap
{

VulkanFence::VulkanFence(VkDevice device, bool createSignaled):
    m_device(device)
{
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if(createSignaled)
    {
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    if(vkCreateFence(m_device, &fenceInfo, nullptr, &m_fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create fence!");
    }
}

VulkanFence::~VulkanFence()
{
    vkDestroyFence(m_device, m_fence, nullptr);
}

VkFence VulkanFence::rawHandle()
{
    return m_fence;
}

void VulkanFence::wait(uint64_t timeout)
{
    vkWaitForFences(m_device, 1, &m_fence, VK_TRUE, timeout);
}

void VulkanFence::resetFence()
{
    vkResetFences(m_device, 1, &m_fence);
}

}
