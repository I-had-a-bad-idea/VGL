#include <vulkan/vulkan.h>
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include <vector>

#include "vk_check.hpp"


typedef struct PhysicalDevice {
    uint32_t device_index;
    VkPhysicalDeviceProperties2 device_properties;
};

std::vector<VkPhysicalDevice> get_devices(VkInstance instance);
PhysicalDevice select_device(std::vector<VkPhysicalDevice> devices, uint32_t preferred_device_index = -1);
