#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>

#include <vector>

#include "vk_check.hpp"


std::vector<VkQueueFamilyProperties> get_queue_families(std::vector<VkPhysicalDevice> devices, uint32_t device_index);
VkDeviceQueueCreateInfo get_queue_family_with_graphics_support(VkInstance instance, std::vector<VkPhysicalDevice> devices, uint32_t device_index,
    std::vector<VkQueueFamilyProperties> queue_families, bool check_presentation_support = true);