#include "renderer.h"

#include <iostream>

int main() {
    std::cout << "Starting...\n";
    Renderer renderer("Test", 1280, 720);
    
    // Wait 5s to see the window
    SDL_Delay(5000);

    std::cout << "Ending...\n";
    return 0;
}
