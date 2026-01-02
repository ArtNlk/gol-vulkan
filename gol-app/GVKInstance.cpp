#include "GVKInstance.h"

GVKInstance GVKInstance::s_instance;

GVKInstance *GVKInstance::i()
{
    return &GVKInstance::s_instance;
}

VkWrap::VulkanInstance *GVKInstance::vki()
{
    return GVKInstance::s_instance.m_vkInstance;
}

void GVKInstance::initialize(int minorApiVersion, std::vector<std::string>& extensions, std::vector<std::string>& validationLayers)
{
    if(m_vkInstance)
    {
        return;
    }

    m_vkInstance = new VkWrap::VulkanInstance(minorApiVersion, extensions, validationLayers);
}

GVKInstance::GVKInstance() :
    m_vkInstance(nullptr)
{

}

GVKInstance::~GVKInstance()
{
    delete m_vkInstance;
}
