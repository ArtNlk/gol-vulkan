#include "VulkanRenderPass.h"

#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace VkWrap
{

VulkanRenderPass::VulkanRenderPass(VkDevice device,
                                   std::vector<VkAttachmentDescription> attachments,
                                   std::vector<VkSubpassDescription> subpasses)
{
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = subpasses.size();
    renderPassInfo.pSubpasses = subpasses.data();

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

VulkanRenderPass::~VulkanRenderPass()
{
    vkDestroyRenderPass(m_device,m_renderPass,nullptr);
}

VkRenderPass VulkanRenderPass::rawHandle() const
{
    return m_renderPass;
}

}
