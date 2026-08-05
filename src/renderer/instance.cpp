#include "renderer.h"

VkInstance Renderer::get_vulkan_instance(std::string app_name) {
    if (app_name.length() == 0) {
        app_name = "VulkanGraphicsLib"; // Name fallback
    }

    // Get application info
    VkApplicationInfo app_info{.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                        .pApplicationName   = app_name.c_str(),
                                        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                        .pEngineName        = "No Engine",
                                        .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
                                        .apiVersion         = VK_API_VERSION_1_4};
    std::cout << "Got app info\n";

    // get plattform specific extensions
    uint32_t instance_extensions_count {0};
    auto instance_extensions {SDL_Vulkan_GetInstanceExtensions(&instance_extensions_count)}; // SDL handles the plattform specific stuff
    
    // std::cout << "SDL extensions: " << instance_extensions_count << "\n";
    // for (uint32_t i = 0; i < instance_extensions_count; i++) {
    //     std::cout << "  " << instance_extensions[i] << "\n";
    // }
    
    VkInstanceCreateInfo instance_CI {};
    instance_CI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_CI.pApplicationInfo = &app_info;
    instance_CI.enabledExtensionCount = instance_extensions_count;
    instance_CI.ppEnabledExtensionNames = instance_extensions;

    // Create instance
    VkInstance instance {VK_NULL_HANDLE};
    VkResult result = vkCreateInstance(&instance_CI, nullptr, &instance);
    // std::cout << "vkCreateInstance returned " << result << std::endl;
    vk_check(result);

    return instance;
}