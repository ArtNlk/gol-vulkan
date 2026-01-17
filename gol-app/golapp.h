#ifndef GOLAPP_H
#define GOLAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "VKWrap.h"

class GolApp
{
public:
    GolApp();
    ~GolApp();

    void run();

protected:
    struct QueueIndicies
    {
        std::optional<uint32_t> graphicQueueIndex;
    };

    void init();
    void initDebugCallback();
    void cleanup();

    void pickPhysicalDevice();
    int rateDeviceSuitability(VkWrap::VulkanPhysicalDevice& dev);

    QueueIndicies getCurrentDeviceIndicies();
    void createLogicalDevice();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    static std::vector<std::string> getRequiredExtensions();

    GLFWwindow* m_window;

    std::shared_ptr<VkWrap::VulkanDebugMessenger> m_debugMessenger;
    std::shared_ptr<VkWrap::VulkanPhysicalDevice> m_physDevice;
    std::shared_ptr<VkWrap::VulkanLogicalDevice> m_device;
};

#endif // GOLAPP_H
