#pragma once

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <string>

#include "vk_check.hpp"

VkInstance get_vulkan_instance(std::string app_name);