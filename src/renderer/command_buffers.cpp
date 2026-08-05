#include "renderer.h"

void Renderer::create_command_buffers() {
    // 1. Create a command pool, since it helps the driver optimize allocations (and since it is required for that reason)
    VkCommandPoolCreateInfo command_pool_CI {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // We reset command buffers before recording them
        .queueFamilyIndex = queue_info.queue_family
    };
    vk_check(vkCreateCommandPool(device, &command_pool_CI, nullptr, &command_pool));

    // 2. Now create the actual command buffers
    VkCommandBufferAllocateInfo cb_alloc_CI {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .commandBufferCount = max_frames_in_flight
    };
    vk_check(vkAllocateCommandBuffers(device, &cb_alloc_CI, command_buffers.data()));
}