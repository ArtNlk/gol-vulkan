#ifndef VULKANIMAGEVIEW_H
#define VULKANIMAGEVIEW_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanImageView
{
public:
    VulkanImageView(VkDevice device, VkImage image, VkFormat format);
    ~VulkanImageView();

protected:
    VkImageView m_view;
    VkDevice m_device;
};

}
#endif // VULKANIMAGEVIEW_H
