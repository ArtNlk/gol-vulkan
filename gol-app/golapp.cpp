#include "golapp.h"
#include "VulkanCommandPool.h"
#include "VulkanPipeline.h"
#include "VulkanPipelineLayout.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderModule.h"
#include "VulkanSwapchain.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <vulkan/vulkan_core.h>

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
        drawFrame();
    }

    m_device->waitIdle();
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

    std::vector<std::string> extensions = getRequiredInstanceExtensions();

    std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    GVKInstance::i()->initialize(3,extensions,validationLayers);

    initDebugCallback();

    createSurface();

    pickPhysicalDevice();

    createLogicalDevice();

    createSwapchain();

    createRenderPass();

    createPipeline();

    createFramebuffers();

    createCommandPool();

    createCommandBuffer();

    createSync();
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

    for(uint32_t i = 0; i < physDevices.size(); i++)
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

    // Application can't function without geometry shaders
    if (!features.geometryShader) {
       return 0;
    }

    if(!getDeviceIndices(dev).complete())
    {
        return 0;
    }

    std::vector<VkExtensionProperties> extensions = dev.getExtensionProperties();

    for(const auto &extensionName : getRequiredDeviceExtensions())
    {
        if(std::none_of(extensions.begin(), extensions.end(), [extensionName](VkExtensionProperties& prop)
            {return std::strcmp(prop.extensionName, extensionName.c_str()) == 0;}))
        {
            return 0;
        }
    }

    VkWrap::VulkanSurface::SwapChainSupportDetails swapChainSupport = m_surface->getDeviceSwapchainCapabilities(dev.rawHandle());
    if(swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
    {
        return 0;
    }

    // Discrete GPUs have a significant performance advantage
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
       score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += properties.limits.maxImageDimension2D;

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

void GolApp::createSwapchain()
{
    VkWrap::VulkanSurface::SwapChainSupportDetails swapChainSupport =
            m_surface->getDeviceSwapchainCapabilities(m_physDevice->rawHandle());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    std::set<uint32_t> families = m_indices.getUniquefamilies();

    std::vector<uint32_t> indices(families.begin(),families.end());

    m_swapchain = std::make_shared<VkWrap::VulkanSwapchain>(m_device->rawHandle(), m_surface->rawHandle(),
                                                            surfaceFormat, presentMode, swapChainSupport.capabilities, extent,
                                                            imageCount, indices);

    m_images = m_swapchain->getImages();
    m_views.reserve(m_images.size());

    for(VkImage img : m_images)
    {
        m_views.emplace_back(m_device->rawHandle(),img, m_swapchain->format());
    }
}

VkSurfaceFormatKHR GolApp::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR GolApp::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D GolApp::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
            return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
}

void GolApp::createRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = m_swapchain->format();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    m_renderPass = std::make_shared<VkWrap::VulkanRenderPass>(m_device->rawHandle(),
                                                              std::vector<VkAttachmentDescription>({colorAttachment}),
                                                              std::vector<VkSubpassDescription>({subpass}),
                                                              std::vector<VkSubpassDependency>({dependency}));
}

void GolApp::createPipeline()
{
    m_pipelineLayout = std::make_shared<VkWrap::VulkanPipelineLayout>(m_device->rawHandle());

    VkWrap::VulkanShaderModule vertModule(m_device->rawHandle(), "./shaders/vertex.vert.spv");
    VkWrap::VulkanShaderModule fragModule(m_device->rawHandle(), "./shaders/fragment.frag.spv");

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_swapchain->extent().width;
    viewport.height = (float) m_swapchain->extent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapchain->extent();

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {vertModule.getStageCreateInfo(), fragModule.getStageCreateInfo()};

    m_pipeline = std::make_shared<VkWrap::VulkanPipeline>(m_device->rawHandle(),
                                                         m_pipelineLayout->rawHandle(),
                                                         m_renderPass->rawHandle(),
                                                         shaderStages,
                                                         vertexInputInfo, inputAssembly,
                                                         viewport, scissor);
}

void GolApp::createFramebuffers()
{
    m_framebuffers.reserve(m_views.size());
    for (size_t i = 0; i < m_views.size(); i++)
    {
        m_framebuffers.emplace_back(m_device->rawHandle(), m_renderPass->rawHandle(),
                                    std::vector<VkImageView>({m_views[i].rawHandle()}), m_swapchain->extent());
    }
}

void GolApp::createCommandPool()
{
    m_commandPool = std::make_shared<VkWrap::VulkanCommandPool>(m_device->rawHandle(),m_indices.graphicFamilyIndex.value());
}

void GolApp::createCommandBuffer()
{
    m_commandBuffer = m_commandPool->createCommandBuffer();
}

void GolApp::createSync()
{
    m_imageAvailableSemaphore = std::make_shared<VkWrap::VulkanSemaphore>(m_device->rawHandle());
    m_renderFinishedSemaphore = std::make_shared<VkWrap::VulkanSemaphore>(m_device->rawHandle());
    m_inFlightFence = std::make_shared<VkWrap::VulkanFence>(m_device->rawHandle(), true);
}

void GolApp::drawFrame()
{
    m_inFlightFence->wait();
    m_inFlightFence->resetFence();

    uint32_t imageIndex = m_swapchain->getNextImage(m_imageAvailableSemaphore->rawHandle());

    m_commandBuffer.resetBuffer();

    recordCommandBuffer(imageIndex);

    m_graphicQueue.submit(m_imageAvailableSemaphore->rawHandle(),m_renderFinishedSemaphore->rawHandle(),
                          m_inFlightFence->rawHandle(),VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                          m_commandBuffer.rawHandle());

    m_presentQueue.present(m_renderFinishedSemaphore->rawHandle(),m_swapchain->rawHandle(), imageIndex);
}

void GolApp::recordCommandBuffer(uint32_t imageIndex)
{
    VkExtent2D swapExtent = m_swapchain->extent();

    m_commandBuffer.beginBuffer();

    m_commandBuffer.beginRenderPass(m_renderPass->rawHandle(), m_framebuffers[imageIndex].rawHandle(),swapExtent);

    m_commandBuffer.bindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->rawHandle());

    m_commandBuffer.setScissor({{0, 0}, {swapExtent}});

    m_commandBuffer.setViewport({0,0,
                                 static_cast<float>(swapExtent.width),
                                 static_cast<float>(swapExtent.height),
                                 0.f, 1.f});

    m_commandBuffer.draw(3);

    m_commandBuffer.endRenderpass();

    m_commandBuffer.endBuffer();
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
    m_indices = getDeviceIndices(*m_physDevice.get());

    std::vector<VkDeviceQueueCreateInfo> queueInfos;

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : m_indices.getUniquefamilies()) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    m_device = std::make_shared<VkWrap::VulkanLogicalDevice>(m_physDevice->rawHandle(),
                                                             queueInfos,
                                                             deviceFeatures,
                                                             getRequiredDeviceExtensions(),
                                                             validationLayers);

    m_graphicQueue = m_device->getQueue(m_indices.graphicFamilyIndex.value());
    m_presentQueue = m_device->getQueue(m_indices.presentFamilyIndex.value());
}

VkBool32 GolApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

std::vector<std::string> GolApp::getRequiredInstanceExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<std::string> extensions;
    extensions.reserve(glfwExtensionCount);

    for(size_t i = 0; i < glfwExtensionCount; i++)
    {
        extensions.emplace_back(glfwExtensions[i]);
    }

    return extensions;
}

std::vector<std::string> GolApp::getRequiredDeviceExtensions()
{
    return {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}
