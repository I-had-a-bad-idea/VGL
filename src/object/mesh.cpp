#include "object.h"

Mesh::Mesh(std::string path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    vk_check(tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, path.c_str())); // load file

    const VkDeviceSize index_count {shapes[0].mesh.indices.size()};
    //load vertex and index data
    for (auto& index : shapes[0].mesh.indices) {
        Vertex v{
            .pos = { attrib.vertices[index.vertex_index * 3], -attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2] },
            .normal = { attrib.normals[index.normal_index * 3], -attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2] },
            .uv = { attrib.texcoords[index.texcoord_index * 2], 1.0 - attrib.texcoords[index.texcoord_index * 2 + 1] }
        };
        vertices.push_back(v);
        indices.push_back(indices.size());
    }
    // y and v axis are flipped to accommodate for Vulkans coordinate system (+y down)
}