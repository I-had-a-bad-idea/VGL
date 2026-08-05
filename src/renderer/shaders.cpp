#include "renderer.h"


void Renderer::create_shader_buffers() {
    for (auto i = 0; i < max_frames_in_flight; i++) {
        VkBufferCreateInfo u_buffer_CI {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = sizeof(ShaderData),
            .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
        };

        VmaAllocationCreateInfo u_buffer_alloc_CI {
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        vk_check(vmaCreateBuffer(allocator, &u_buffer_CI, &u_buffer_alloc_CI, &shader_data_buffers[i].buffer, &shader_data_buffers[i].allocation, &shader_data_buffers[i].allocation_info));

        // store device address for later
        VkBufferDeviceAddressInfo u_buffer_bda_info {
            .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
            .buffer = shader_data_buffers[i].buffer
        };
        shader_data_buffers[i].deviceAddress = vkGetBufferDeviceAddress(device, &u_buffer_bda_info);

    }
}

void Renderer::create_slang_session() {
    // INit Slang shader compiler
    slang::createGlobalSession(slang_global_session.writeRef());

    auto slang_targets {std::to_array<slang::TargetDesc>({ {
        .format {SLANG_SPIRV},
        .profile {slang_global_session->findProfile("spirv_1_4")}
    } })};

    auto slang_options {std::to_array<slang::CompilerOptionEntry>({ {
        slang::CompilerOptionName::EmitSpirvDirectly,
        {slang::CompilerOptionValueKind::Int, 1}
    } })};

    slang::SessionDesc slang_session_desc {
        .targets {slang_targets.data()},
        .targetCount {SlangInt(slang_targets.size())},
        .defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
        .compilerOptionEntries {slang_options.data()},
        .compilerOptionEntryCount {uint32_t(slang_options.size())}
    };

    slang_global_session->createSession(slang_session_desc, slang_session.writeRef());
}