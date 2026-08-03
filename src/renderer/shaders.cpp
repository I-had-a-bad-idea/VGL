#include "renderer.h"


void Renderer::create_shader_buffers() {
    for (auto i = 0; i < max_frames_in_flight; i++) {
        VkBufferCreateInfo u_buffer_CI {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = sizeof(ShaderData),
            .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        };

        VmaAllocationCreateInfo u_buffer_alloc_CI {
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        vk_check(vmaCreateBuffer(allocator, &u_buffer_CI, &u_buffer_alloc_CI, &shader_data_buffers[i].buffer, &shader_data_buffers[i].allocation, &shader_data_buffers[i].allocationInfo));

        // store device address for later
        VkBufferDeviceAddressInfo u_buffer_bda_info {
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = shader_data_buffers[i].buffer
        };
        shader_data_buffers[i].deviceAddress = vkGetBufferDeviceAddress(device, &u_buffer_bda_info);

    }
}