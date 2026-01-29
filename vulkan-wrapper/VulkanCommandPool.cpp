#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"

#include <stdexcept>

namespace VkWrap
{

VulkanCommandPool::VulkanCommandPool(VkDevice device, uint32_t queueFamilyIndex):
    m_device(device)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

VulkanCommandPool::~VulkanCommandPool()
{
    vkDestroyCommandPool(m_device, m_pool, nullptr);
}

VulkanCommandBuffer VulkanCommandPool::createCommandBuffer()
{
    return VkWrap::VulkanCommandBuffer(m_device, m_pool);
}

}
