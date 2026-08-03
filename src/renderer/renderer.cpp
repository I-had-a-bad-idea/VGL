#include "renderer.h"


Renderer::Renderer(std::string name, int w, int h) {
    std::cout << "Initializing up SDL...\n";
    init_sdl();
    std::cout << "Initializing up Volk...\n";
    init_volk();

    std::cout << "Creating Vulkan instance...\n";
    instance = get_vulkan_instance(name);
    volk_load_instance(instance);

    std::cout << "Getting physical device...\n";
    physical_devices = get_devices();
    physical_device = select_device();

    std::cout << "Getting queue family...\n";
    std::vector<VkQueueFamilyProperties> queue_families = get_queue_families();
    queue_info = get_queue_family_with_graphics_support(queue_families);

    std::cout << "Creating logical device...\n";
    device = create_logical_device(queue_info.queue_CI);
    graphics_queue = get_device_queue(queue_info.queue_family);

    std::cout << "Setting up VMA...\n";
    allocator = setup_vma();

    std::cout << "Creating window...\n";
    window = create_window(name, w, h);
    window_data = create_vulkan_surface_for_window();
    VkSurfaceCapabilitiesKHR surface_caps = get_surface_properties();

    std::cout << "Creating swapchain...\n";
    swapchain = create_swapchain(surface_caps);
    swapchain_data = get_swapchain_data();

    std::cout << "Depth attachment...\n";
    depth_format = get_depth_format();
    create_depth_image_and_depth_image_view();

    std::cout << "Creaing shader buffers...\n";
    create_shader_buffers();

    std::cout << "Creating sync objects (fences, semaphores)...\n";
    create_sync_objects();

    std::cout << "Creating command buffers...\n";
    create_command_buffers();
}