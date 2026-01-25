#ifndef VULKANGRAPHICSPIPELINE_H
#define VULKANGRAPHICSPIPELINE_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VkWrap
{

class VulkanPipeline
{
public:
    VulkanPipeline(VkDevice device, VkPipelineLayout pipelineLayout,
                   VkRenderPass renderPass,
                   std::vector<VkPipelineShaderStageCreateInfo> shaderStages,
                   VkPipelineVertexInputStateCreateInfo vertexInfo,
                   VkPipelineInputAssemblyStateCreateInfo inputAssembly,
                   VkViewport viewport, VkRect2D scissor);
    ~VulkanPipeline();

protected:
    VkPipeline m_pipeline;
    VkDevice m_device;
};

}
#endif // VULKANGRAPHICSPIPELINE_H
