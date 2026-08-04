#include "object.h"


Texture::Texture(std::string path) {
    ktxTexture_CreateFromNamedFile(path.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktx_texture); // load texture from file
}

void Texture::load_image_into_buffer(VmaAllocator allocator) {
    VkImageCreateInfo texture_image_CI {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = ktxTexture_GetVkFormat(ktx_texture),
        .extent {.width = ktx_texture->baseWidth, .height = ktx_texture->baseHeight, .depth = 1},
        .mipLevels = ktx_texture->numLevels,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };

    VmaAllocationCreateInfo texture_image_alloc_CI {.usage = VMA_MEMORY_USAGE_AUTO};
    vk_check(vmaCreateImage(allocator, &texture_image_CI, &texture_image_alloc_CI, &image, &allocation, nullptr));
}
