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

    SDL_Window* window = SDL_CreateWindow(name.c_str(), w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    assert(window);

    return window;
}


WindowData create_vulkan_surface_for_window(VkInstance instance, SDL_Window* window) {
    WindowData window_data;

    VkSurfaceKHR surface {VK_NULL_HANDLE};
    vk_check(SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface));
    window_data.surface = surface;

    vk_check(SDL_GetWindowSize(window, &window_data.x, &window_data.y));

    return window_data;
}

VkSurfaceCapabilitiesKHR get_surface_properties(std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkSurfaceKHR surface) {
    VkSurfaceCapabilitiesKHR surface_caps{};
    vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[device_index], surface, &surface_caps));

    return surface_caps;
}
