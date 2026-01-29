#ifndef VULKANCOMMANDBUFFER_H
#define VULKANCOMMANDBUFFER_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanCommandBuffer
{
public:
    VulkanCommandBuffer();

    VulkanCommandBuffer(VkDevice device, VkCommandPool pool);

    void beginBuffer();

    void beginRenderPass(VkRenderPass renderPass,
                         VkFramebuffer frameBuffer,
                         VkExtent2D renderArea,
                         VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}});

    void bindPipeline(VkPipelineBindPoint bindpoint, VkPipeline pipeline);

    void setViewport(VkViewport viewport);

    void setScissor(VkRect2D rect);

    void draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0);

    void endRenderpass();

    void endBuffer();

protected:
    VkCommandBuffer m_buffer;
    VkDevice m_device;
};

}
#endif // VULKANCOMMANDBUFFER_H
