#include "VulkanCommandBuffer.h"

#include <stdexcept>

namespace VkWrap
{

VulkanCommandBuffer::VulkanCommandBuffer() :
    m_buffer(VK_NULL_HANDLE),
    m_device(VK_NULL_HANDLE)
{

}

VulkanCommandBuffer::VulkanCommandBuffer(VkDevice device, VkCommandPool pool) :
    m_device(device)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(m_device, &allocInfo, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VulkanCommandBuffer::beginBuffer()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(m_buffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
}

void VulkanCommandBuffer::beginRenderPass(VkRenderPass renderPass,
                                          VkFramebuffer frameBuffer,
                                          VkExtent2D renderArea,
                                          VkClearValue clearColor)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = frameBuffer;

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = renderArea;

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(m_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::bindPipeline(VkPipelineBindPoint bindpoint, VkPipeline pipeline)
{
    vkCmdBindPipeline(m_buffer, bindpoint, pipeline);
}

void VulkanCommandBuffer::setViewport(VkViewport viewport)
{
    vkCmdSetViewport(m_buffer, 0, 1, &viewport);
}

void VulkanCommandBuffer::setScissor(VkRect2D rect)
{
    vkCmdSetScissor(m_buffer, 0, 1, &rect);
}

void VulkanCommandBuffer::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(m_buffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanCommandBuffer::endRenderpass()
{
    vkCmdEndRenderPass(m_buffer);
}

void VulkanCommandBuffer::endBuffer()
{
    if (vkEndCommandBuffer(m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

}
