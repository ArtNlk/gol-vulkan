#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#include <vulkan/vulkan.h>

#include <vector>

namespace VkWrap
{

class VulkanRenderPass
{
public:
    VulkanRenderPass(VkDevice device,
                     std::vector<VkAttachmentDescription> attachments,
                     std::vector<VkSubpassDescription> subpasses);
    ~VulkanRenderPass();

    VkRenderPass rawHandle() const;

protected:
    VkRenderPass m_renderPass;
    VkDevice m_device;
};

}
#endif // VULKANRENDERPASS_H
