#include "device.h"


std::vector<VkPhysicalDevice> get_devices(VkInstance instance) {
    uint32_t device_count {0};

    vk_check(vkEnumeratePhysicalDevices(instance, &device_count, nullptr));
    std::vector<VkPhysicalDevice> devices(device_count);
    vk_check(vkEnumeratePhysicalDevices(instance, &device_count, devices.data())); // get devices

    return devices;
}

PhysicalDevice select_device(std::vector<VkPhysicalDevice> devices, uint32_t preferred_device_index = -1) {
    uint32_t device_count = devices.size();

    uint32_t device_index {0};
    if (preferred_device_index != -1) {
        device_index = preferred_device_index;
        assert(device_index < device_count);
    }

    // get device info
    VkPhysicalDeviceProperties2 device_properties {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
    vkGetPhysicalDeviceProperties2(devices[device_index], &device_properties);
    std::cout << "Selected device: " << device_properties.properties.deviceName << "\n";

    return PhysicalDevice {.device_index = device_index, .device_properties = device_properties};
    
}