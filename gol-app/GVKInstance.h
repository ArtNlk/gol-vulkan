#ifndef GVKINSTANCE_H
#define GVKINSTANCE_H
#include "VKWrap.h"

class GVKInstance
{
public:
    static GVKInstance* i();

    static VkWrap::VulkanInstance* vki();

    void initialize(int minorApiVersion, std::vector<std::string> &extensions,
                    std::vector<std::string> &validationLayers);

protected:
    GVKInstance();
    ~GVKInstance();

    static GVKInstance s_instance;

    VkWrap::VulkanInstance* m_vkInstance;
};

#endif // GVKINSTANCE_H
