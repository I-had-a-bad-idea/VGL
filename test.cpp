#include "renderer.h"
#include "object.h"

#include <iostream>


int main() {
    std::cout << "Starting...\n";
    Renderer renderer("Test", 1280, 720);
    Scene scene;
    
    // Load meshes, textures and shaders
    Mesh monkey_mesh = renderer.load_mesh("assets/monkey.obj");
    Texture gravel_texture = renderer.load_texture("assets/Textures/Gravel.ktx");
    Shader shader = renderer.load_shader("assets/shader.slang");
    // Create a material for gravel
    Material gravel_material(&gravel_texture, &shader);

    // Create a monkey object
    Object monkey(&monkey_mesh, &gravel_material);

    // Add objects to scene
    scene.add_object_to_scene(monkey);

    // while (1) {
    //     renderer.render_scene(scene); // Render the scene
    // }

    // Destroy resources
    renderer.destroy_mesh(monkey_mesh);
    renderer.destroy_texture(gravel_texture);
    renderer.destroy_shader(shader);

    renderer.destroy();

    // Wait 5s to see the window
    SDL_Delay(5000);

    std::cout << "Ending...\n";
    return 0;
}
