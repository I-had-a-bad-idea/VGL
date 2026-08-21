#include "VGL/renderer.h"

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

    uint32_t present_mode_count = 0;
    vk_check(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_devices[device_index], window_data.surface, &present_mode_count, nullptr));
    std::vector<VkPresentModeKHR> present_modes(present_mode_count);
    vk_check(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_devices[device_index], window_data.surface, &present_mode_count, present_modes.data()));

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (VkPresentModeKHR candidate : {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR}) { // these modes allow for 3-buffering or no waiting at all (so not dependent on monitor refresh rate)
        for (VkPresentModeKHR supported_mode : present_modes) {
            if (supported_mode == candidate) {
                present_mode = candidate;
                break;
            }
        }
        if (present_mode != VK_PRESENT_MODE_FIFO_KHR) {
            break;
        }
    }

    uint32_t desired_image_count = surface_caps.minImageCount + 1; // alway 1 more, which gives the GPU a place to put next image instead of waiting
    if (surface_caps.maxImageCount > 0 && desired_image_count > surface_caps.maxImageCount) {
        desired_image_count = surface_caps.maxImageCount;
    }

    image_format = VkFormat {VK_FORMAT_B8G8R8A8_SRGB};
    swapchain_CI = VkSwapchainCreateInfoKHR {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = window_data.surface,
        .minImageCount = desired_image_count,
        .imageFormat = image_format,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent {.width = swapchain_extent.width, .height = swapchain_extent.height},
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode
    };
    
    VkSwapchainKHR swapchain {VK_NULL_HANDLE};
    vk_check(vkCreateSwapchainKHR(device, &swapchain_CI, nullptr, &swapchain));

    return swapchain;
}

Renderer::SwapchainData Renderer::get_swapchain_data() {
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;

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

VkFormat Renderer::get_depth_format() {
    std::vector<VkFormat> depth_format_list {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
    VkFormat depth_format {VK_FORMAT_UNDEFINED};

    for (VkFormat& format : depth_format_list) {
        VkFormatProperties2 format_properties {.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2};
        vkGetPhysicalDeviceFormatProperties2(physical_devices[device_index], format, &format_properties);
        if (format_properties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depth_format = format;
            break;
        }
    }
    assert(depth_format != VK_FORMAT_UNDEFINED);

    return depth_format;
}

void Renderer::create_depth_image_and_depth_image_view() {
    depth_image_CI = VkImageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = depth_format,
		.extent{.width = static_cast<uint32_t>(window_data.x), .height = static_cast<uint32_t>(window_data.y), .depth = 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL, // make sure the format (of image storing) is best suited for the GPU
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, // will be used as the depth attachment for render output
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VmaAllocationCreateInfo alloc_CI {.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT, .usage = VMA_MEMORY_USAGE_AUTO};
    vk_check(vmaCreateImage(allocator, &depth_image_CI, &alloc_CI, &depth_image, &depth_image_allocation, nullptr));

    VkImageViewCreateInfo depth_view_CI {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .image = depth_image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = depth_format,
        .subresourceRange {.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1}
    };
    vk_check(vkCreateImageView(device, &depth_view_CI, nullptr, &depth_image_view));
}


void Renderer::update_swapchain() {
    swapchain_update_required = false;
    vk_check(vkDeviceWaitIdle(device));
    vk_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_devices[device_index], window_data.surface, &surface_caps));
    swapchain_CI.oldSwapchain = swapchain;
    swapchain_CI.imageExtent = {.width = static_cast<uint32_t>(window_data.x), .height = static_cast<uint32_t>(window_data.y)};
    vk_check(vkCreateSwapchainKHR(device, &swapchain_CI, nullptr, &swapchain)); // recreate swapchain

    for (auto i = 0; i < image_count; i++) {
        vkDestroyImageView(device, swapchain_data.swapchain_image_views[i], nullptr);
    }
    vk_check(vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr));
    swapchain_data.swapchain_images.resize(image_count);
    vk_check(vkGetSwapchainImagesKHR(device, swapchain, &image_count, swapchain_data.swapchain_images.data()));
    swapchain_data.swapchain_image_views.resize(image_count);

    for (auto i = 0; i < image_count; i++) {
        VkImageViewCreateInfo view_CI {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapchain_data.swapchain_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = image_format,
            .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1}
        };
        vk_check(vkCreateImageView(device, &view_CI, nullptr, &swapchain_data.swapchain_image_views[i]));
    }

    // Recreate semaphores
    for (auto& semaphore : render_complete_semaphores) {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
    render_complete_semaphores.resize(image_count);
    for (auto& semaphore : render_complete_semaphores) {
        vk_check(vkCreateSemaphore(device, &semaphore_CI, nullptr, &semaphore));
    }

    vkDestroySwapchainKHR(device, swapchain_CI.oldSwapchain, nullptr);
    vmaDestroyImage(allocator, depth_image, depth_image_allocation);
    vkDestroyImageView(device, depth_image_view, nullptr);
    depth_image_CI.extent = {.width = static_cast<uint32_t>(window_data.x), .height = static_cast<uint32_t>(window_data.y), .depth = 1};

    VmaAllocationCreateInfo alloc_CI {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    vk_check(vmaCreateImage(allocator, &depth_image_CI, &alloc_CI, &depth_image, &depth_image_allocation, nullptr));

    VkImageViewCreateInfo view_CI {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = depth_image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = depth_format,
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT, .levelCount = 1, .layerCount = 1 }
    };

    vk_check(vkCreateImageView(device, &view_CI, nullptr, &depth_image_view));
}