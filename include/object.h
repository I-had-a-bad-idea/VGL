#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <tinyobj/tiny_obj_loader.h>

#include <string>
#include <vector>

#include "vk_check.hpp"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
    public:
        Mesh(std::string filepath);
        std::vector<Vertex> vertices {};
        std::vector<uint16_t> indices {};
};

class Texture {
    public:
        Texture(std::string filepath);
};


class Object {
    public:
        Mesh mesh;
        Texture texture;

        Object(std::string path_to_mesh, std::string path_to_texture);

        void load_mesh_into_buffer(VmaAllocator allocator);
    private:
        VmaAllocation v_buffer_allocation{ VK_NULL_HANDLE };
        VkBuffer v_buffer{ VK_NULL_HANDLE };
};
