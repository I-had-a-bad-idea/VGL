#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "vk_check.hpp"

void init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    vk_check(SDL_Vulkan_LoadLibrary(nullptr)); // Load Vulkan library
}