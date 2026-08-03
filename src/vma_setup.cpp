#include "renderer.h"

VmaAllocator Renderer::setup_vma() {
    VmaVulkanFunctions vkFunctions{
        .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = vkGetDeviceProcAddr,
        .vkCreateImage = vkCreateImage
    };
    VmaAllocatorCreateInfo allocatorCI{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT, 
        .physicalDevice = physical_devices[device_index],
        .device = device,
        .pVulkanFunctions = &vkFunctions,
        .instance = instance
    };

    VmaAllocator allocator {VK_NULL_HANDLE};
    vk_check(vmaCreateAllocator(&allocatorCI, &allocator));
    return allocator;
}