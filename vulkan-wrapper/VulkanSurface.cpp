#include "VulkanSurface.h"

VkWrap::VulkanSurface::VulkanSurface(VkSurfaceKHR surface, VkInstance instance) :
    m_surface(surface),
    m_instance(instance)
{

}

VkWrap::VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

VkSurfaceKHR VkWrap::VulkanSurface::rawHandle()
{
    return m_surface;
}
