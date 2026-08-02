#include "vma_setup.h"

VmaAllocator setup_vma(VkInstance instance, std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkDevice device) {
    VmaVulkanFunctions vkFunctions{
        .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = vkGetDeviceProcAddr,
        .vkCreateImage = vkCreateImage
    };
    VmaAllocatorCreateInfo allocatorCI{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT, 
        .physicalDevice = devices[device_index],
        .device = device,
        .pVulkanFunctions = &vkFunctions,
        .instance = instance
    };

    VmaAllocator allocator {VK_NULL_HANDLE};
    vk_check(vmaCreateAllocator(&allocatorCI, &allocator));
    return allocator;
}