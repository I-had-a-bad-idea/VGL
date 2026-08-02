#pragma once

#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>

#include <string>
#include <vector>
#include <cassert>

#include "vk_check.hpp"

struct WindowData {
    VkSurfaceKHR surface;
    int x;
    int y;
};

SDL_Window* create_window(std::string name, int w, int h);
WindowData create_vulkan_surface_for_window(VkInstance instance, SDL_Window* window);
VkSurfaceCapabilitiesKHR get_surface_properties(std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkSurfaceKHR surface);
