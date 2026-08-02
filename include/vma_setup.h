#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#include <vector>

#include "vk_check.hpp"

VmaAllocator setup_vma(VkInstance instance, std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkDevice device);