#ifndef VULKANFRAMEBUFFER_H
#define VULKANFRAMEBUFFER_H

#include <vector>
#include <vulkan/vulkan.h>
#include <vector>

namespace VkWrap
{

class VulkanFramebuffer
{
public:
    VulkanFramebuffer(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView> attachments, VkExtent2D size);
    ~VulkanFramebuffer();

protected:
    VkFramebuffer m_framebuffer;
    VkDevice m_device;
};

}
#endif // VULKANFRAMEBUFFER_H
