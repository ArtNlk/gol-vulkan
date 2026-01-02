#ifndef VULKANDEBUGMESSENGER_H
#define VULKANDEBUGMESSENGER_H

#include <vulkan/vulkan.h>

namespace VkWrap
{

class VulkanDebugMessenger
{   
public:
    using SeverityBits = VkDebugUtilsMessageSeverityFlagBitsEXT;
    using MessageType = VkDebugUtilsMessageTypeFlagsEXT;

    VulkanDebugMessenger(VkInstance instance, PFN_vkDebugUtilsMessengerCallbackEXT callback,
                         SeverityBits severity,
                         MessageType type);

    ~VulkanDebugMessenger();

protected:
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator);


    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkInstance m_parentInstance;
};

}
#endif // VULKANDEBUGMESSENGER_H
