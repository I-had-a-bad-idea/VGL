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


void Mesh::load_mesh_into_buffer(VmaAllocator allocator) {
    VkDeviceSize v_buffer_size {sizeof(Vertex) * vertices.size()};
    VkDeviceSize i_buffer_size {sizeof(uint16_t) * indices.size()};

    VkBufferCreateInfo buffer_CI {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = v_buffer_size + i_buffer_size, // since we combine both into one buffer
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT // tell GPU, that we are combining the buffers
    };

    VmaAllocationCreateInfo v_buffer_alloc_CI {
        // make sure we get memory, that on the GPU (in VRAM) and accessible by host
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };
    VmaAllocationInfo v_buffer_alloc_info {};
    vk_check(vmaCreateBuffer(allocator, &buffer_CI, &v_buffer_alloc_CI, &v_buffer, &v_buffer_allocation, &v_buffer_alloc_info));
    
    // copy data into buffer
    memcpy(v_buffer_alloc_info.pMappedData, vertices.data(), v_buffer_size);
    memcpy(((char*)v_buffer_alloc_info.pMappedData) + v_buffer_size, indices.data(), i_buffer_size);
}
