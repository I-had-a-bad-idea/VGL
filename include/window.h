#pragma once

#include <volk/volk.h>
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

struct SwapchainData {
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;
};

SDL_Window* create_window(std::string name, int w, int h);
WindowData create_vulkan_surface_for_window(VkInstance instance, SDL_Window* window);
VkSurfaceCapabilitiesKHR get_surface_properties(std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkSurfaceKHR surface);
VkSwapchainKHR create_swapchain(VkDevice device, WindowData window_data, VkSurfaceCapabilitiesKHR surface_caps);
SwapchainData get_swapchain_data(VkDevice device, VkSwapchainKHR swapchain, VkFormat image_format);