#pragma once

#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include "vk_check.hpp"
#include "sdl.hpp"
#include "volk.hpp"

class Renderer {
public:
    Renderer(std::string name, int w, int h);

private:
    struct QueueInfo {
        uint32_t queue_family;
        VkDeviceQueueCreateInfo queue_CI;
    };

    struct PhysicalDevice {
        uint32_t device_index;
        VkPhysicalDeviceProperties2 device_properties;
    };

    struct WindowData {
        VkSurfaceKHR surface;
        int x;
        int y;
    };

    struct SwapchainData {
        std::vector<VkImage> swapchain_images;
        std::vector<VkImageView> swapchain_image_views;
    };

private:
    // Instance helpers
    VkInstance get_vulkan_instance(std::string app_name);

    // Physical device
    std::vector<VkPhysicalDevice> get_devices();
    PhysicalDevice select_device(uint32_t preferred_device_index = UINT32_MAX);

    // Queues
    std::vector<VkQueueFamilyProperties> get_queue_families();
    QueueInfo get_queue_family_with_graphics_support(std::vector<VkQueueFamilyProperties> queue_families, bool check_presentation_support = true);

    // Logical device
    VkDevice create_logical_device(VkDeviceQueueCreateInfo queue_CI);
    VkQueue get_device_queue(uint32_t queue_family);

    // Window / Surface
    SDL_Window* create_window(std::string name, int w, int h);
    WindowData create_vulkan_surface_for_window();
    VkSurfaceCapabilitiesKHR get_surface_properties();
    VkSwapchainKHR create_swapchain(VkSurfaceCapabilitiesKHR surface_caps);
    SwapchainData get_swapchain_data();

    // Depth attachment
    VkFormat get_depth_format();
    void create_depth_image_and_depth_image_view();

    // Memory
    VmaAllocator setup_vma();

private:
    VkInstance instance = VK_NULL_HANDLE;

    std::vector<VkPhysicalDevice> physical_devices;
    uint32_t device_index = 0;
    PhysicalDevice physical_device;

    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphics_queue = VK_NULL_HANDLE;

    SDL_Window* window = nullptr;
    WindowData window_data;

    VkFormat image_format;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    SwapchainData swapchain_data;

    VmaAllocator allocator = VK_NULL_HANDLE;

    VkFormat depth_format;
    VkImage depth_image;
    VmaAllocation depth_image_allocation;
    VkImageView depth_image_view;
};