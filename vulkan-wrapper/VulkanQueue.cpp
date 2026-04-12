#include "VulkanQueue.h"

#include <stdexcept>

namespace VkWrap
{

VulkanQueue::VulkanQueue(VkQueue queue) :
    m_queue(queue)
{

}

void VulkanQueue::submit(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore, VkFence fence, VkPipelineStageFlags stages, VkCommandBuffer commandBuffer)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &waitSemaphore;
    submitInfo.pWaitDstStageMask = &stages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &signalSemaphore;

    if (vkQueueSubmit(m_queue, 1, &submitInfo, fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
}

VkResult VulkanQueue::present(VkSemaphore semaphore, VkSwapchainKHR swapchain, uint32_t imageIdx)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &semaphore;

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &imageIdx;

    presentInfo.pResults = nullptr; // Optional

    return vkQueuePresentKHR(m_queue, &presentInfo);
}

}
