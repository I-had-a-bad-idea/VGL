#include "renderer.h"


void Renderer::create_texture_descriptors() {
    VkDescriptorBindingFlags desc_variable_flag {VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT};
    VkDescriptorSetLayoutBindingFlagsCreateInfo  desc_binding_flags {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
        .bindingCount = 1,
        .pBindingFlags = &desc_variable_flag
    };
    VkDescriptorSetLayoutBinding desc_layout_binding_text {
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = max_textures,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
    };
    VkDescriptorSetLayoutCreateInfo desc_layout_text_CI {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = &desc_binding_flags,
        .bindingCount = 1,
        .pBindings = &desc_layout_binding_text
    };
    vk_check(vkCreateDescriptorSetLayout(device, &desc_layout_text_CI, nullptr, &descriptor_set_layout_tex));

    VkDescriptorPoolSize pool_size {
        .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = max_textures
    };
    VkDescriptorPoolCreateInfo desc_pool_CI {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 1,
        .poolSizeCount = 1,
        .pPoolSizes = &pool_size
    };
    vk_check(vkCreateDescriptorPool(device, &desc_pool_CI, nullptr, &descriptor_pool));

    // Allocate the descriptor set from the pool
    uint32_t variable_desc_count = max_textures;
    VkDescriptorSetVariableDescriptorCountAllocateInfo variable_desc_count_AI {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT,
        .descriptorSetCount = 1,
        .pDescriptorCounts = &variable_desc_count
    };
    VkDescriptorSetAllocateInfo text_desc_set_alloc {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = &variable_desc_count_AI,
        .descriptorPool = descriptor_pool,
        .descriptorSetCount = 1,
        .pSetLayouts = &descriptor_set_layout_tex
    };

    vk_check(vkAllocateDescriptorSets(device, &text_desc_set_alloc, &descriptor_set_tex));

}

void Renderer::update_texture_descriptor(uint32_t index, Texture& texture) {
    VkDescriptorImageInfo image_info {
        .sampler = texture.sampler,
        .imageView = texture.view,
        .imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL
    };

    VkWriteDescriptorSet write_desc_set {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = descriptor_set_tex,
        .dstBinding = 0,
        .dstArrayElement = index, // modify only this texture slot
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = &image_info
    };

    vkUpdateDescriptorSets(device, 1, &write_desc_set, 0, nullptr);
}