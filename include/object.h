#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

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

    private:
        std::vector<Vertex> vertices {};
        std::vector<uint16_t> indices {};
};