#ifndef GOLAPP_H
#define GOLAPP_H

#include "VulkanDebugMessenger.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>

class GolApp
{
public:
    GolApp();
    ~GolApp();

    void run();

protected:
    void init();
    void initDebugCallback();
    void cleanup();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    static std::vector<std::string> getRequiredExtensions();

    GLFWwindow* m_window;

    std::shared_ptr<VkWrap::VulkanDebugMessenger> m_debugMessenger;
};

#endif // GOLAPP_H
