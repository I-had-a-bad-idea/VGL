#include <vulkan/vulkan.h>
#include <SDL3/SDL_vulkan.h>

#include <string>
#include <vector>

#include "vk_check.hpp"

SDL_Window* create_window(std::string name, int w, int h);
VkSurfaceKHR create_vulkan_surface_for_window(VkInstance instance, SDL_Window* window);
VkSurfaceCapabilitiesKHR get_surface_properties(std::vector<VkPhysicalDevice> devices, uint32_t device_index, VkSurfaceKHR surface);
