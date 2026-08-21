#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "VGL/vk_check.hpp"

static inline void init_sdl() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        exit(-1);
    }

    if (!SDL_Vulkan_LoadLibrary(nullptr)) {
        std::cerr << "SDL_Vulkan_LoadLibrary failed: " 
                  << SDL_GetError() << "\n";
        exit(-1);
    }
}