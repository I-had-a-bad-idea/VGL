#include <vulkan/vulkan.h>
#include <iostream>

static inline void vk_check(VkResult result) {
	if (result != VK_SUCCESS) {
		std::cerr << "Vulkan call returned an error (" << result << ")\n";
		exit(result);
	}