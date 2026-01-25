#include "VulkanShaderModule.h"

#include <format>
#include <stdexcept>
#include <fstream>
#include <vector>

namespace VkWrap
{

VulkanShaderModule::VulkanShaderModule(VkDevice device, const std::string &shaderPath):
    m_device(device)
{
    std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(std::format("failed to open file! {}",shaderPath));
    }

    size_t fileSize = (size_t) file.tellg();
    m_buffer.resize(fileSize);

    file.seekg(0);
    file.read(m_buffer.data(), fileSize);
    file.close();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = m_buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(m_buffer.data());

    if (vkCreateShaderModule(device, &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_createInfo.stage = shaderPath.ends_with(".vert")? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
    m_createInfo.module = m_shaderModule;
    m_createInfo.pName = "main";
}

VulkanShaderModule::~VulkanShaderModule()
{
    vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
}

VkShaderModule VulkanShaderModule::rawHandle() const
{
    return m_shaderModule;
}

VkPipelineShaderStageCreateInfo VulkanShaderModule::getStageCreateInfo()
{
    return m_createInfo;
}

}
