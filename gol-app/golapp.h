#ifndef GOLAPP_H
#define GOLAPP_H

#include "VulkanPipeline.h"
#include "VulkanPipelineLayout.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <set>

#include "VKWrap.h"

class GolApp
{
public:
    GolApp();
    ~GolApp();

    void run();

protected:
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicFamilyIndex;
        std::optional<uint32_t> presentFamilyIndex;

        bool complete() const
        {
            return graphicFamilyIndex.has_value() && presentFamilyIndex.has_value();
        }

        std::set<uint32_t> getUniquefamilies()
        {
            if(!complete())
            {
                return {};
            }

            return {graphicFamilyIndex.value(),presentFamilyIndex.value()};
        }
    };

    void init();
    void initDebugCallback();
    void cleanup();

    void pickPhysicalDevice();
    int rateDeviceSuitability(VkWrap::VulkanPhysicalDevice& dev);

    QueueFamilyIndices getDeviceIndices(VkWrap::VulkanPhysicalDevice& dev);
    void createLogicalDevice();

    void createSurface();

    void createSwapchain();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createRenderPass();

    void createPipeline();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    static std::vector<std::string> getRequiredInstanceExtensions();

    static std::vector<std::string> getRequiredDeviceExtensions();

    GLFWwindow* m_window;

    std::shared_ptr<VkWrap::VulkanDebugMessenger> m_debugMessenger;
    std::shared_ptr<VkWrap::VulkanPhysicalDevice> m_physDevice;
    std::shared_ptr<VkWrap::VulkanLogicalDevice> m_device;
    std::shared_ptr<VkWrap::VulkanSurface> m_surface;
    std::shared_ptr<VkWrap::VulkanSwapchain> m_swapchain;

    std::vector<VkImage> m_images;
    std::vector<VkWrap::VulkanImageView> m_views;

    QueueFamilyIndices m_indices;
    VkWrap::VulkanQueue m_graphicQueue;
    VkWrap::VulkanQueue m_presentQueue;

    std::shared_ptr<VkWrap::VulkanRenderPass> m_renderPass;
    std::shared_ptr<VkWrap::VulkanPipelineLayout> m_pipelineLayout;
    std::shared_ptr<VkWrap::VulkanPipeline> m_pipeline;
};

#endif // GOLAPP_H
