#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <tinyobj/tiny_obj_loader.h>
#include <ktx.h>
#include <ktxvulkan.h>

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

        void load_mesh_into_buffer(VmaAllocator allocator);

    private:
        std::vector<Vertex> vertices {};
        std::vector<uint16_t> indices {};

        VmaAllocation v_buffer_allocation{ VK_NULL_HANDLE };
        VkBuffer v_buffer{ VK_NULL_HANDLE };
};

class Texture {
    public:
        Texture(std::string filepath);

        void load_image_into_buffer(VkDevice device, VkCommandPool command_pool, VkQueue graphics_queue, VmaAllocator allocator);

    private:
    	VmaAllocation allocation{ VK_NULL_HANDLE };
	    VkImage image{ VK_NULL_HANDLE };
	    VkImageView view{ VK_NULL_HANDLE };
	    VkSampler sampler{ VK_NULL_HANDLE };
        ktxTexture* ktx_texture {nullptr};
};

class Object {
    public:
        Mesh mesh;
        Texture texture;

        void load_into_buffers(VkDevice device, VkCommandPool command_pool, VkQueue graphics_queue, VmaAllocator allocator);
        Object(std::string path_to_mesh, std::string path_to_texture);

};
