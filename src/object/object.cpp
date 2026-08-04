#include "object.h"

Object::Object(std::string path_to_mesh, std::string path_to_texture)
    : mesh(path_to_mesh),
    texture(path_to_texture)
{}

void Object::load_into_buffers(VmaAllocator allocator) {
    mesh.load_mesh_into_buffer(allocator);
    texture.load_image_into_buffer(allocator);
    std::cout << "Loaded object into buffers!\n";
}