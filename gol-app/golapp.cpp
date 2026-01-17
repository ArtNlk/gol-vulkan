#include "golapp.h"

#include <algorithm>
#include <format>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "GVKInstance.h"

GolApp::GolApp() :
    m_window(nullptr)
{
    init();
}

GolApp::~GolApp()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GolApp::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

void GolApp::init()
{
    glfwInit();

    if (glfwVulkanSupported())
    {
        std::cout << "GLFW detected vulkan support\n";
    }
    else
    {
        throw std::runtime_error("GLFW could not detect vulkan support!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(800,600,"GOL-app",nullptr,nullptr);

    std::vector<VkLayerProperties> availableLayers = VkWrap::VulkanInstance::getLayers();

    std::cout << "Found " << availableLayers.size() << " layers\n";

    for(auto& layer : availableLayers)
    {
        std::cout << layer.layerName << ' ' << layer.description << "\n";
    }

    std::cout.flush();

    std::vector<std::string> extensions = getRequiredExtensions();

    std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    GVKInstance::i()->initialize(3,extensions,validationLayers);

    initDebugCallback();

    createSurface();

    pickPhysicalDevice();

    createLogicalDevice();
}

void GolApp::initDebugCallback()
{
    using SeverityBits = VkWrap::VulkanDebugMessenger::SeverityBits;
    using MessageType = VkWrap::VulkanDebugMessenger::MessageType;

    SeverityBits messageSeverity = static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(
                //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

    MessageType messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    m_debugMessenger = std::make_shared<VkWrap::VulkanDebugMessenger>(GVKInstance::vki()->rawInstance(),
                                                                     GolApp::debugCallback,
                                                                     messageSeverity,
                                                                     messageType);
}

void GolApp::cleanup()
{
    m_debugMessenger.reset();
}

void GolApp::pickPhysicalDevice()
{
    std::vector<VkWrap::VulkanPhysicalDevice> physDevices = GVKInstance::vki()->getPhysicalDevices();

    if(physDevices.empty())
    {
        throw std::runtime_error("No vulkan devices found!");
    }

    int bestScore = 0;

    int bestIdx = -1;

    for(int i = 0; i < physDevices.size(); i++)
    {
        int score = rateDeviceSuitability(physDevices[i]);
        if(score > bestScore)
        {
            bestScore = score;
            bestIdx = i;
        }
    }

    if(bestScore == 0)
    {
        throw std::runtime_error("No suitable physical device found!");
    }

    m_physDevice = std::make_shared<VkWrap::VulkanPhysicalDevice>(physDevices[bestIdx]);
}

int GolApp::rateDeviceSuitability(VkWrap::VulkanPhysicalDevice &dev)
{
    int score = 0;

    VkPhysicalDeviceFeatures features = dev.getFeatures();
    VkPhysicalDeviceProperties properties = dev.getProperties();

    if(!getDeviceIndices(dev).complete())
    {
        return 0;
    }

    // Discrete GPUs have a significant performance advantage
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
       score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += properties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    if (!features.geometryShader) {
       return 0;
    }

    return score;
}

void GolApp::createSurface()
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkResult res;

    if ((res = glfwCreateWindowSurface(GVKInstance::vki()->rawInstance(), m_window, nullptr, &surface)) != VK_SUCCESS)
    {
        throw std::runtime_error(std::format("failed to create window surface! Code {}",static_cast<int>(res)));
    }

    m_surface = std::make_shared<VkWrap::VulkanSurface>(surface, GVKInstance::vki()->rawInstance());
}

GolApp::QueueFamilyIndices GolApp::getDeviceIndices(VkWrap::VulkanPhysicalDevice& dev)
{
    QueueFamilyIndices idx;

    std::vector<VkQueueFamilyProperties> queueFamilies = dev.getQueueProperties();

    for(uint32_t i = 0; i < queueFamilies.size(); i++)
    {
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            idx.graphicFamilyIndex = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(dev.rawHandle(), i, m_surface->rawHandle(), &presentSupport);

        if (presentSupport) {
            idx.presentFamilyIndex = i;
        }
    }

    return idx;
}

void GolApp::createLogicalDevice()
{
    QueueFamilyIndices indices = getDeviceIndices(*m_physDevice.get());

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicFamilyIndex.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    queueInfos.push_back(queueCreateInfo);

    m_device = std::make_shared<VkWrap::VulkanLogicalDevice>(m_physDevice->rawHandle(),queueInfos,deviceFeatures,validationLayers);
}

VkBool32 GolApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

std::vector<std::string> GolApp::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<std::string> extensions;
    extensions.reserve(glfwExtensionCount);

    for(size_t i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(std::string(glfwExtensions[i]));
    }

    return extensions;
}
