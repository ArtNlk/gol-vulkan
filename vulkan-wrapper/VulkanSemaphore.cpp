#include "VulkanSemaphore.h"

#include <stdexcept>

namespace VkWrap
{

VulkanSemaphore::VulkanSemaphore(VkDevice device):
    m_device(device)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphore!");
    }
}

VulkanSemaphore::~VulkanSemaphore()
{
    vkDestroySemaphore(m_device, m_semaphore, nullptr);
}

VkSemaphore VulkanSemaphore::rawHandle() const
{
    return m_semaphore;
}

}
