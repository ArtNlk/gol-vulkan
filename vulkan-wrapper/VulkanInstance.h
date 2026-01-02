#include <string>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <vector>
#include <memory>

namespace VkWrap
{

class VulkanInstance
{
public:
    VulkanInstance(int minorApiVersion, std::vector<std::string> &extensions,
                   std::vector<std::string> &validationLayers);
    ~VulkanInstance();

    static std::vector<VkExtensionProperties> getProperties();

    static std::vector<VkLayerProperties> getLayers();

    std::vector<VkPhysicalDevice> getPhysicalDevices();

    void filterValidationLayers(std::vector<std::string>& layers);

    VkInstance rawInstance();

protected:
    bool checkLayerAvailability(const std::vector<std::string> layersToCheck);

    VkInstance m_vkInstance;
};

}
