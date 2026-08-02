#include "queues.h"


std::vector<VkQueueFamilyProperties> get_queue_families(std::vector<VkPhysicalDevice> devices, uint32_t device_index) {
    uint32_t queue_family_count{ 0 };
    vkGetPhysicalDeviceQueueFamilyProperties(devices[device_index], &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(devices[device_index], &queue_family_count, queue_families.data()); // get all families

    return queue_families;
}


QueueInfo get_queue_family_with_graphics_support(VkInstance instance, std::vector<VkPhysicalDevice> devices, uint32_t device_index,
    std::vector<VkQueueFamilyProperties> queue_families, bool check_presentation_support = true) {
    
    uint32_t queue_family {0};

    for (size_t i = 0; i < queue_families.size(); i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { // check for grahics support
            queue_family = i;
            break;
        }
    }

    if (check_presentation_support) {
        vk_check(SDL_Vulkan_GetPresentationSupport(instance, devices[device_index], queue_family)); // check for presentation support
    }

    // Referebce the queue.
    const float qf_priorities{ 1.0f };
    VkDeviceQueueCreateInfo queue_CI{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queue_family,
        .queueCount = 1,
        .pQueuePriorities = &qf_priorities
    };

    return QueueInfo {.queue_family = queue_family, .queue_CI = queue_CI};
}