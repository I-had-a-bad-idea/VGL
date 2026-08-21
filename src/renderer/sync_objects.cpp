#include "VGL/renderer.h"

void Renderer::create_sync_objects() {
    // Creates fences and semaphores
    // Fences: Signal work completion from GPU to CPU
    // Semaphores: Control access to resources on the GPU (only)

    semaphore_CI = VkSemaphoreCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };
    VkFenceCreateInfo fence_CI {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT // create in signaled state, otherwise it would run into timeout on first wait
    };

    for (auto i = 0; i < max_frames_in_flight; i++) {
        vk_check(vkCreateFence(device, &fence_CI, nullptr, &fences[i]));
        vk_check(vkCreateSemaphore(device, &semaphore_CI, nullptr, &image_acquired_semaphores[i]));
    }
    render_complete_semaphores.resize(swapchain_data.swapchain_images.size());
    for (auto& semaphore : render_complete_semaphores) {
        vk_check(vkCreateSemaphore(device, &semaphore_CI, nullptr, &semaphore));
    }
}