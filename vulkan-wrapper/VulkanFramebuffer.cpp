#include "VulkanFramebuffer.h"

#include <stdexcept>

namespace VkWrap
{

VulkanFramebuffer::VulkanFramebuffer(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView> attachments, VkExtent2D size)
{
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = attachments.size();
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = size.width;
    framebufferInfo.height = size.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create framebuffer!");
    }
}

VulkanFramebuffer::~VulkanFramebuffer()
{
    vkDestroyFramebuffer(m_device, m_framebuffer, nullptr);
}

}
