#include "renderer.h"


std::vector<VkPhysicalDevice> Renderer::get_devices() {
    uint32_t device_count {0};

    // std::cout << "Instance: " << instance << std::endl;

    vk_check(vkEnumeratePhysicalDevices(instance, &device_count, nullptr));
    std::vector<VkPhysicalDevice> devices(device_count);
    vk_check(vkEnumeratePhysicalDevices(instance, &device_count, devices.data())); // get devices

    return devices;
}

Renderer::PhysicalDevice  Renderer::select_device(uint32_t preferred_device_index) {
    uint32_t device_count = physical_devices.size();

    if (preferred_device_index != UINT32_MAX) {
        device_index = preferred_device_index;
        assert(device_index < device_count);
    }

    // get device info
    VkPhysicalDeviceProperties2 device_properties {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
    vkGetPhysicalDeviceProperties2(physical_devices[device_index], &device_properties);
    std::cout << "Selected device: " << device_properties.properties.deviceName << "\n";

    return PhysicalDevice {.device_index = device_index, .device_properties = device_properties};
    
}