#ifndef VULKANSEMAPHORE_H
#define VULKANSEMAPHORE_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanSemaphore
{
public:
    VulkanSemaphore(VkDevice device);
    ~VulkanSemaphore();

    VkSemaphore rawHandle() const;

protected:
    VkSemaphore m_semaphore;
    VkDevice m_device;
};

}
#endif // VULKANSEMAPHORE_H
