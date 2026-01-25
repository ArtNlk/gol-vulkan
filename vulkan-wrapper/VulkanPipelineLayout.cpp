#include "VulkanPipelineLayout.h"

#include <stdexcept>

namespace VkWrap
{

VulkanPipelineLayout::VulkanPipelineLayout(VkDevice device):
    m_device(device)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional


    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
    vkDestroyPipelineLayout(m_device, m_layout, nullptr);
}

VkPipelineLayout VulkanPipelineLayout::rawHandle()
{
    return m_layout;
}

}
