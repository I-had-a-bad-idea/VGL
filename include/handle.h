#pragma once

#include <volk/volk.h>

#include <vector>

#include "vk_check.hpp"


VkDevice create_logical_device(std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkDeviceQueueCreateInfo queue_CI);
VkQueue get_device_queue(VkDevice device, uint32_t queue_family);