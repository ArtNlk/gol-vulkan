#include "VulkanInstance.h"
#include <algorithm>
#include <vulkan/vulkan.h>

#include <stdexcept>
#include <format>
#include <cstring>
#include <vulkan/vulkan_core.h>

namespace VkWrap
{

VulkanInstance::VulkanInstance(int minorApiVersion, std::vector<std::string>& extensions,
                               std::vector<std::string>& validationLayers) :
    m_vkInstance(VK_NULL_HANDLE)
{
    const bool validationLayersEnabled = !validationLayers.empty();

    if(validationLayersEnabled && !checkLayerAvailability(validationLayers))
    {
        throw std::runtime_error("Some requested validation layers unavailable!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "GOL app";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    switch(minorApiVersion)
    {
    case 0:
        appInfo.apiVersion = VK_API_VERSION_1_0;
        break;

    case 1:
        appInfo.apiVersion = VK_API_VERSION_1_1;
        break;

    case 2:
        appInfo.apiVersion = VK_API_VERSION_1_2;
        break;

    case 3:
        appInfo.apiVersion = VK_API_VERSION_1_3;
        break;

    default:
        appInfo.apiVersion = VK_API_VERSION_1_0;
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> cstrExtenstions;
    for(size_t i = 0; i < cstrExtenstions.size(); i++)
    {
        cstrExtenstions[i] = extensions[i].c_str();
    }
    if(validationLayersEnabled)
    {
        cstrExtenstions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    createInfo.enabledExtensionCount = cstrExtenstions.size();
    createInfo.ppEnabledExtensionNames = cstrExtenstions.data();

    std::vector<const char*> cstrLayers(validationLayers.size());
    for(size_t i = 0; i < validationLayers.size(); i++)
    {
        cstrLayers[i] = validationLayers[i].c_str();
    }

    if (validationLayersEnabled)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(cstrLayers.size());
        createInfo.ppEnabledLayerNames = cstrLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    const VkResult res = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);
    if (res != VK_SUCCESS) {
        throw std::runtime_error(std::format("Failed to create vulkan instance! Code {}",static_cast<int>(res)));
    }
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(m_vkInstance, nullptr);
}

std::vector<VkExtensionProperties> VulkanInstance::getProperties()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    return extensions;
}

std::vector<VkLayerProperties> VulkanInstance::getLayers()
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    return availableLayers;
}

std::vector<VkPhysicalDevice> VulkanInstance::getPhysicalDevices()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        return std::vector<VkPhysicalDevice>();
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    return devices;
}

VkInstance VulkanInstance::rawInstance()
{
    return m_vkInstance;
}

bool VulkanInstance::checkLayerAvailability(const std::vector<std::string> layersToCheck)
{
    std::vector<VkLayerProperties> availableLayers = getLayers();

    for(const std::string& checkedLayer : layersToCheck)
    {
        if(std::none_of(availableLayers.begin(), availableLayers.end(),
                        [checkedLayer](const VkLayerProperties& availableLayer)
                        {return std::strcmp(checkedLayer.c_str(),availableLayer.layerName) == 0;}))
        {
            return false;
        }
    }

    return true;
}

}
