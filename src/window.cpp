#include "renderer.h"

SDL_Window* Renderer::create_window(std::string name, int w, int h) {
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


Renderer::WindowData Renderer::create_vulkan_surface_for_window() {
    WindowData window_data;

    VkSurfaceKHR surface {VK_NULL_HANDLE};
    vk_check(SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface));
    window_data.surface = surface;

    vk_check(SDL_GetWindowSize(window, &window_data.x, &window_data.y));

    return window_data;
}

VkSurfaceCapabilitiesKHR Renderer::get_surface_properties() {
    VkSurfaceCapabilitiesKHR surface_caps{};
    vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_devices[device_index], window_data.surface, &surface_caps));

    return surface_caps;
}

VkSwapchainKHR Renderer::create_swapchain(VkSurfaceCapabilitiesKHR surface_caps) {
    VkExtent2D swapchain_extent{ surface_caps.currentExtent };
    if (surface_caps.currentExtent.width == 0xFFFFFFFF) { // special value indicating, that the surace size will be determined by window size
        swapchain_extent = { .width = static_cast<uint32_t>(window_data.x), .height = static_cast<uint32_t>(window_data.y) };
    }
    image_format = VkFormat {VK_FORMAT_B8G8R8A8_SRGB};
    VkSwapchainCreateInfoKHR swapchain_CI {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = window_data.surface,
        .minImageCount = surface_caps.minImageCount,
        .imageFormat = image_format,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent {.width = swapchain_extent.width, .height = swapchain_extent.height},
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR // v-synced mode (only mode guaranteed)
    };
    
    VkSwapchainKHR swapchain {VK_NULL_HANDLE};
    vk_check(vkCreateSwapchainKHR(device, &swapchain_CI, nullptr, &swapchain));

    return swapchain;
}

Renderer::SwapchainData Renderer::get_swapchain_data() {
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;

    uint32_t image_count {0};
    vk_check(vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr));
    swapchain_images.resize(image_count);
    vk_check(vkGetSwapchainImagesKHR(device, swapchain, &image_count, swapchain_images.data()));
    swapchain_image_views.resize(image_count);
	for (auto i = 0; i < image_count; i++) {
		VkImageViewCreateInfo viewCI{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapchain_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = image_format,
            .subresourceRange {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1}
        };
		vk_check(vkCreateImageView(device, &viewCI, nullptr, &swapchain_image_views[i]));
	}

    return SwapchainData {.swapchain_images = swapchain_images, .swapchain_image_views = swapchain_image_views};
}