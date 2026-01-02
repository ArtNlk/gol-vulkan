#include "golapp.h"
#include "VulkanDebugMessenger.h"
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
}

void GolApp::initDebugCallback()
{
    using SeverityBits = VkWrap::VulkanDebugMessenger::SeverityBits;
    using MessageType = VkWrap::VulkanDebugMessenger::MessageType;

    SeverityBits messageSeverity = static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
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

VkBool32 GolApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

std::vector<std::string> GolApp::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<std::string> extensions(glfwExtensionCount);

    for(size_t i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(std::string(glfwExtensions[i]));
    }

    return extensions;
}
