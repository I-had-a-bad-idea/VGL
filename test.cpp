#include "renderer.h"
#include "object.h"

#include <iostream>


int main() {
    std::cout << "Starting...\n";
    Renderer renderer("Test", 1280, 720);
    Scene scene;
    
    // Load meshes, textures and shaders
    std::cout << "Loading resources...\n";
    Mesh monkey_mesh = renderer.load_mesh("assets/monkey.obj");
    Texture gravel_texture = renderer.load_texture("assets/Textures/Gravel.ktx");
    std::cout << "Loading shader...\n";
    Shader shader = renderer.load_shader("assets/shader.slang");
    std::cout << "Creating material...\n";
    // Create a material for gravel
    Material gravel_material(&gravel_texture, &shader);

    // Create a monkey object
    Object monkey(&monkey_mesh, &gravel_material);

    std::cout << "Adding object(s) to scene...\n";
    // Add objects to scene
    scene.add_object_to_scene(&monkey);
    scene.add_object_to_scene(&monkey);

    std::cout << "Starting rendering..." << std::endl;
    uint64_t last_time{ SDL_GetTicks() };
    uint64_t fps_update_time{ last_time };
    uint32_t frame_count{ 0 };
    bool quit{ false };
    while (!quit) {
        uint64_t now = SDL_GetTicks();
        float elapsed_time{ (now - last_time) / 1000.0f };
        last_time = now;

        renderer.render_scene(scene); // Render the scene
        ++frame_count;

        if (now - fps_update_time >= 1000) {
            float fps{ static_cast<float>(frame_count) / ((now - fps_update_time) / 1000.0f) };
            std::cout << "FPS: " << fps << '\n';
            frame_count = 0;
            fps_update_time = now;
        }

        monkey.rotation.y += elapsed_time;

        for (SDL_Event event; SDL_PollEvent(&event);) {
            // Exit loop if the application is about to close
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }

            // Zooming with the mouse wheel
            if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                scene.cam_pos.z += (float)event.wheel.y * elapsed_time * 100.0f;
            }
        }
    }

    // Destroy resources
    renderer.destroy_mesh(monkey_mesh);
    renderer.destroy_texture(gravel_texture);
    renderer.destroy_shader(shader);

    renderer.destroy();



    std::cout << "Ending...\n";
    return 0;
}
