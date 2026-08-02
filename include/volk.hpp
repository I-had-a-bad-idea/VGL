#pragma once

#include <vulkan/vulkan.h>
#include <volk/volk.h>

#include "vk_check.hpp"

static inline void init_volk() {
    vk_check(volkInitialize());
}

static inline void volk_load_instance(VkInstance instance) {
    volkLoadInstance(instance);
}