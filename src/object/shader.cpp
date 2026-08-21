#include "VGL/object.h"


std::string load_shader_source(const std::string& filepath) {
// Open the file using ifstream
    std::ifstream file(filepath);

    // confirm file opening
    if (!file.is_open()) {
        // print error message and return
        std::cerr << "Failed to open file: " << filepath << std::endl;
    }

    // Read the file line by line into a string
    std::string shader_source;
    std::string line;
    while (std::getline(file, line)) {
        shader_source += line + "\n";
    }
    // Close the file
    file.close();
    return shader_source;
}

Shader::Shader(std::string filepath, VkDevice device, Slang::ComPtr<slang::ISession> slang_session) {
    // First read shader from disk into a buffer
    std::string shader_source = load_shader_source(filepath);

    // Get filename as shader name
    std::filesystem::path shader_path(filepath);
    std::string shader_name = shader_path.stem().string();

    Slang::ComPtr<ISlangBlob> diagnostics;
    Slang::ComPtr<slang::IModule> slang_module {
        slang_session->loadModuleFromSourceString(shader_name.c_str(), filepath.c_str(), shader_source.c_str(), diagnostics.writeRef())
    };
    if (!slang_module) {
        if (diagnostics) {
            std::cerr << static_cast<const char*>(diagnostics->getBufferPointer()) << '\n';
        }
    }

    Slang::ComPtr<ISlangBlob> spirv;
    slang_module->getTargetCode(0, spirv.writeRef());

    VkShaderModuleCreateInfo shader_module_CI {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = spirv->getBufferSize(),
        .pCode = (uint32_t*)spirv->getBufferPointer()
    };

    vk_check(vkCreateShaderModule(device, &shader_module_CI, nullptr, &shader_module));
}

void Shader::destroy(VkDevice device) {
    vkDestroyPipelineLayout(device, graphics_pipeline.layout, nullptr);
	vkDestroyPipeline(device, graphics_pipeline.pipeline, nullptr);
	vkDestroyShaderModule(device, shader_module, nullptr);
}
