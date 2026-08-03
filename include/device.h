#pragma once

#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>

#include <vector>
#include <iostream>
#include <cassert>

#include "vk_check.hpp"


struct PhysicalDevice {
    uint32_t device_index;
    VkPhysicalDeviceProperties2 device_properties;
};

std::vector<VkPhysicalDevice> get_devices(VkInstance instance);
PhysicalDevice select_device(std::vector<VkPhysicalDevice> devices, uint32_t preferred_device_index = UINT32_MAX);
