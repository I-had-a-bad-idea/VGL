#include "renderer.h"
#include "object.h"

#include <iostream>

int main() {
    std::cout << "Starting...\n";
    Renderer renderer("Test", 1280, 720);
    
    Mesh monkey("assets/monkey.obj");

    // Wait 5s to see the window
    SDL_Delay(5000);

    std::cout << "Ending...\n";
    return 0;
}
