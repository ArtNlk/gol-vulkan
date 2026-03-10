#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VkWrap
{

class VulkanQueue
{
public:
    VulkanQueue(VkQueue queue = VK_NULL_HANDLE);

    void submit(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                VkFence fence, VkPipelineStageFlags stages,
                VkCommandBuffer commandBuffer);

    void present(VkSemaphore semaphore, VkSwapchainKHR swapchain, uint32_t imageIdx);

protected:
    VkQueue m_queue;
};

}
#endif // VULKANQUEUE_H
