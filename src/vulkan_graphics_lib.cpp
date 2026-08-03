#include "vulkan_graphics_lib.h"

Renderer create_renderer(std::string name, int w, int h) {
    std::cout << "Initializing up SDL...\n";
    init_sdl();
    std::cout << "Initializing up Volk...\n";
    init_volk();

    std::cout << "Creating Vulkan instance...\n";
    VkInstance instance = get_vulkan_instance(name);
    volk_load_instance(instance);

    std::cout << "Getting physical device...\n";
    std::vector<VkPhysicalDevice> devices = get_devices(instance);
    PhysicalDevice physical_device = select_device(devices);

    std::cout << "Getting queue family...\n";
    std::vector<VkQueueFamilyProperties> queue_families = get_queue_families(devices, physical_device.device_index);
    QueueInfo queue_info = get_queue_family_with_graphics_support(instance, devices, physical_device.device_index, queue_families);

    std::cout << "Creating logical device...\n";
    VkDevice device = create_logical_device(devices, physical_device.device_index, queue_info.queue_CI);
    VkQueue queue = get_device_queue(device, queue_info.queue_family);

    std::cout << "Setting up VMA...\n";
    VmaAllocator allocator = setup_vma(instance, devices, physical_device.device_index, device);

    std::cout << "Creating window...\n";
    SDL_Window* window = create_window(name, w, h);
    WindowData window_data = create_vulkan_surface_for_window(instance, window);
    VkSurfaceCapabilitiesKHR surface_caps = get_surface_properties(devices, physical_device.device_index, window_data.surface);

    std::cout << "Creating swapchain...\n";
    VkSwapchainKHR swapchain = create_swapchain(device, window_data, surface_caps);
// VkSwapchainKHR create_swapchain(VkDevice device, WindowData window_data, VkSurfaceCapabilitiesKHR surface_caps);
// SwapchainData get_swapchain_images(VkDevice device, VkSwapchainKHR swapchain, VkFormat image_format);

    return Renderer();
}