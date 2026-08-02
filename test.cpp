#include "vulkan_graphics_lib.h"

#include <iostream>

int main() {
    std::cout << "Starting...\n";
    Renderer renderer = create_renderer("Test", 1280, 720);
    
    std::cout << "Ending...\n";
    return 0;
}
