#include <vulkan/vulkan.h>
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include <vector>

#include "vk_check.hpp"


typedef struct PhysicalDevice {
    uint32_t device_index;
    VkPhysicalDeviceProperties2 device_properties;
};


PhysicalDevice select_device(VkInstance instance, uint32_t preferred_device_index = -1);
