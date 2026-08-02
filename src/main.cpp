#include "main.h"

int main() {
    VkInstance instance = get_vulkan_instance("VulkanRenderLib");
    std::vector<VkPhysicalDevice> devices = get_devices(instance);
    PhysicalDevice device = select_device(devices);

    std::vector<VkQueueFamilyProperties> queue_families = get_queue_families(devices, device.device_index);

    VkDeviceQueueCreateInfo queue_CI = get_queue_family_with_graphics_support(instance, devices, device.device_index, queue_families);

    return 0;
}