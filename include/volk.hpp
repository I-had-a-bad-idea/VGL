#pragma once

#include <vulkan/vulkan.h>
#include <volk/volk.h>

#include "vk_check.hpp"

void init_volk() {
    volkInitialize();
}

void volk_load_instance(VkInstance instance) {
    volkLoadInstance(instance);
}