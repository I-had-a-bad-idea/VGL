# VulkanGraphicsLib
A vulkan graphics library (may be extended with compute)

A more detailed README will be added once it is in a usable state.

## Overview
- [VulkanGraphicsLib](#vulkangraphicslib)
  - [Overview](#overview)
  - [Including the library](#including-the-library)
  - [Usage](#usage)
    - [Axes](#axes)
    - [Example file](#example-file)
  - [Licenses for textures under asses/Textures](#licenses-for-textures-under-assestextures)


## Including the library

1. Download the [latest release](https://github.com/I-had-a-bad-idea/VulkanGraphicsLib/releases/latest)
2. Unpack the zip
3. Set your compiler to include the `include/` directory (`-Ipath/to/folder/include`)
4. Link the `VulkanGraphicsLib` (`-lVulkanGraphicsLib`)
5. [Use](#usage) it for your project

## Usage

### Axes
> Beware once the camera is rotated (currently not supported) this will change

- -z is forward
- +x is right
- +y is up (as it should be)


### Example file

> See the `test.cpp` file

A minimal usage file could look like this
```cpp

// Includes from the VulkanGraphicsLib
#include "renderer.h"
#include "object.h"

#include <iostream>


int main() {
    std::cout << "Starting...\n";
    Renderer renderer("Test", 1280, 720); // Create a renderer with window dimensions
    Scene scene; // A scene is used to group objects
    
    // Load meshes, textures and shaders
    std::cout << "Loading resources...\n";
    Mesh monkey_mesh = renderer.load_mesh("assets/monkey.obj"); // Currently only .obj is supported
    Texture gravel_texture = renderer.load_texture("assets/Textures/Gravel.ktx"); // Currently only .ktx (as it is a format the GPU likes)
    std::cout << "Loading shader...\n";
    Shader shader = renderer.load_shader("assets/shader.slang"); // The slang compiler is included in the library and shaders will be compiled when loaded
    std::cout << "Creating material...\n";
    // Create a material for gravel
    Material gravel_material(&gravel_texture, &shader); // Create a material from a texture and a shader

    // Create object(s) (mesh, material, position, rotation)
    Object monkey(&monkey_mesh, &gravel_material, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

    std::cout << "Adding object(s) to scene...\n";
    // Add objects to scene
    scene.add_object_to_scene(&monkey);

    std::cout << "Starting rendering..." << std::endl;
    uint64_t last_time{ SDL_GetTicks() }; // this is only FPS metrics related stuff
    bool quit{ false };
    while (!quit) {
        uint64_t now = SDL_GetTicks();
        float elapsed_time{ (now - last_time) / 1000.0f };
        last_time = now;

        renderer.render_scene(scene); // Render the scene

        // Update your scene (e.g. rotate the monkeys)
        monkey.rotation.z += elapsed_time;

        for (SDL_Event event; SDL_PollEvent(&event);) {
            // Exit loop if the application is about to close
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }

            // Zooming with the mouse wheel (add your movement here [currently rotation is not supported])
            if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                scene.cam_pos.z += (float)event.wheel.y * elapsed_time * 100.0f;
            }
        }
    }

    // Destroy resources (you are fully responsible for the objects)
    renderer.destroy_mesh(monkey_mesh);
    renderer.destroy_texture(gravel_texture);
    renderer.destroy_shader(shader);

    renderer.destroy();

    // Alternativly let the OS do this (not nice or clean)

    std::cout << "Ending...\n";
    return 0;
}

}
```


## Licenses for textures under asses/Textures
<Gravel 041> <Grass 005> from ambientCG.com, licensed under the Creative Commons CC0 1.0 Universal License.         
Links to the textures on ambientCG.com are next to the textures.