#include "renderer.h"
#include "object.h"

#include <iostream>


int main() {
    std::cout << "Starting...\n";
    Renderer renderer("Test", 1280, 720);
    
    // Load meshes and textures
    Mesh monkey_mesh = renderer.load_mesh("assets/monkey.obj");
    Texture gravel_texture = renderer.load_texture("assets/Textures/Gravel.ktx");
    // Create a material for gravel
    Material gravel_material(&gravel_texture);

    // Create a monkey object
    Object monkey(&monkey_mesh, &gravel_material);

    // Wait 5s to see the window
    SDL_Delay(5000);

    std::cout << "Ending...\n";
    return 0;
}
