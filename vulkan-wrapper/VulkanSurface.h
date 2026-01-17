#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanSurface
{
public:
    VulkanSurface(VkSurfaceKHR surface, VkInstance instance);
    ~VulkanSurface();

    VkSurfaceKHR rawHandle();

protected:
    VkSurfaceKHR m_surface;
    VkInstance m_instance;
};

}
#endif // VULKANSURFACE_H
