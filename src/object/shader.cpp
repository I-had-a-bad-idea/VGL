#include "object.h"


Shader::Shader(std::string filepath, VkDevice device, Slang::ComPtr<slang::ISession> slang_session) {
    Slang::ComPtr<slang::IModule> slang_module {
        slang_session->loadModuleFromSource("triangle", filepath.c_str(), nullptr, nullptr)
    };
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
