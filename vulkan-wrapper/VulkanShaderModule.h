#ifndef VULKANSHADERMODULE_H
#define VULKANSHADERMODULE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace VkWrap
{

class VulkanShaderModule
{
public:
    VulkanShaderModule(VkDevice device, const std::string& shaderPath);
    ~VulkanShaderModule();

    VkShaderModule rawHandle() const;

    VkPipelineShaderStageCreateInfo getStageCreateInfo();

protected:
    VkPipelineShaderStageCreateInfo m_createInfo;
    VkShaderModule m_shaderModule;
    VkDevice m_device;
    std::vector<char> m_buffer;
};

}
#endif // VULKANSHADERMODULE_H
