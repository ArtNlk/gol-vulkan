#ifndef VULKANPIPELINELAYOUT_H
#define VULKANPIPELINELAYOUT_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanPipelineLayout
{
public:
    VulkanPipelineLayout(VkDevice device);
    ~VulkanPipelineLayout();

    VkPipelineLayout rawHandle();

protected:
    VkPipelineLayout m_layout;
    VkDevice m_device;
};

}
#endif // VULKANPIPELINELAYOUT_H
