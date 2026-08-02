#include "instance.h"

VkInstance get_vulkan_instance(std::string app_name) {
    if (app_name.length() == 0) {
        app_name = "VulkanGraphicsLib"; // Name fallback
    }

    // Get application info
    VkApplicationInfo app_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = app_name.c_str(),
        .apiVersion = VK_API_VERSION_1_3
    };

    // get plattform specific extensions
    uint32_t instance_extensions_count {0};
    char const* const* instance_extensions {SDL_Vulkan_GetInstanceExtensions(&instance_extensions_count)}; // SDL handles the plattform specific stuff
    

    VkInstanceCreateInfo instance_CI{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_info,
        .enabledExtensionCount = instance_extensions_count,
        .ppEnabledExtensionNames = instance_extensions,
    };
    // Create instance
    VkInstance instance {VK_NULL_HANDLE};
    vk_check(vkCreateInstance(&instance_CI, nullptr, &instance));

    return instance;
}