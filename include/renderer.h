#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <array>

#include "vk_check.hpp"
#include "sdl.hpp"
#include "volk.hpp"
#include "object.h"

class Renderer {
public:
    static constexpr uint32_t max_frames_in_flight = 2;
    Renderer(std::string name, int w, int h);

    void load_object(Object object);

private:
    struct ShaderData {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model[3];
        glm::vec4 lightPos{ 0.0f, -10.0f, 10.0f, 0.0f };
        uint32_t selected{1};
    } shaderData{};

    struct ShaderDataBuffer {
        VmaAllocation allocation{ VK_NULL_HANDLE };
        VmaAllocationInfo allocationInfo{};
        VkBuffer buffer{ VK_NULL_HANDLE };
        VkDeviceAddress deviceAddress{};
    };

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

    // Shaders
    void create_shader_buffers();

    // Sync objects
    void create_sync_objects();
    
    // Command pool and buffers
    void create_command_buffers();

private:
    VkInstance instance = VK_NULL_HANDLE;

    std::vector<VkPhysicalDevice> physical_devices;
    uint32_t device_index = 0;
    PhysicalDevice physical_device;

    VkDevice device = VK_NULL_HANDLE;
    QueueInfo queue_info;
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

    std::array<ShaderDataBuffer, max_frames_in_flight> shader_data_buffers;

    std::array<VkFence, max_frames_in_flight> fences;
    std::array<VkSemaphore, max_frames_in_flight> image_acquired_semaphores;
    std::vector<VkSemaphore> render_complete_semaphores;

    VkCommandPool command_pool{ VK_NULL_HANDLE };
    std::array<VkCommandBuffer, max_frames_in_flight> command_buffers;
};