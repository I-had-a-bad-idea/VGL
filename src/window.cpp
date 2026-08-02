#include "window.h"

SDL_Window* create_window(std::string name, int w, int h) {
    if (name.length() == 0) {
        name = "VulkanGraphicsLib"; // Name fallback
    }
    // dimension fallbacks
    if (w == 0) {
        w = 1280u; 
    }
    if (h == 0) {
        h = 720u;
    }

    return SDL_CreateWindow(name.c_str(), w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
}


VkSurfaceKHR create_vulkan_surface_for_window(VkInstance instance, SDL_Window* window) {
    VkSurfaceKHR surface {VK_NULL_HANDLE};
    vk_check(SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface));
}

VkSurfaceCapabilitiesKHR get_surface_properties(std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkSurfaceKHR surface) {
    VkSurfaceCapabilitiesKHR surface_caps{};
    vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[device_index], surface, &surface_caps));

    return surface_caps;
}
