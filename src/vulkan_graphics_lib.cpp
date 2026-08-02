#include "vulkan_graphics_lib.h"

Renderer create_renderer(std::string name) {
    VkInstance instance = get_vulkan_instance(name);
    std::vector<VkPhysicalDevice> devices = get_devices(instance);
    PhysicalDevice physical_device = select_device(devices);

    std::vector<VkQueueFamilyProperties> queue_families = get_queue_families(devices, physical_device.device_index);

    QueueInfo queue_info = get_queue_family_with_graphics_support(instance, devices, physical_device.device_index, queue_families);

    VkDevice device = create_logical_device(devices, physical_device.device_index, queue_info.queue_CI);
    VkQueue queue = get_device_queue(device, queue_info.queue_family);

    VmaAllocator allocator = setup_vma(instance, devices, physical_device.device_index, device);

    return Renderer();
}