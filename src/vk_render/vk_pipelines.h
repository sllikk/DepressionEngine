#pragma once

#include "vk_initializers.h"
#include "vk_images.h"
#include "vk_types.h"

#include <vector>

class VulkanRender;

namespace vkutil {

	bool load_shader_module(const char* filePath, VkDevice device, VkShaderModule* outShaderModule);
//    const AllocatedImage load_texture(const char* texture_path, bool mipmapped, VulkanRender& render);

}


class PipelineBuilder {

public:

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;
    VkPipelineRenderingCreateInfo _renderInfo;
    VkFormat _colorAttachmentformat;

    PipelineBuilder() { clear(); }

public:
   
    void clear();
    void set_input_topology(VkPrimitiveTopology topology);
    void set_polygon_mode(VkPolygonMode mode);
    void set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace);
    void set_multisampling_none();
    void disable_blending();
    void enable_blending_additive();
    void enable_blending_alphablend();
    void set_color_attachment_format(VkFormat format);
    void set_depth_format(VkFormat format);
    void disable_depthtest();
    void enable_depthtest(bool depthWriteEnable, VkCompareOp op);


    VkPipeline build_pipeline(VkDevice device, VkRenderPass renderPass);  // for vulkan 1.2 render pass 
    VkPipeline build_pipeline(VkDevice device); // that for vulkan 1.3 and dynamic render


    void set_shaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
    


};


// Use This pipeline without PVP + VertexBufferAdress 
class OldPipelineBuilder {

public:

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;
    VkPipelineRenderingCreateInfo _renderInfo;
    VkFormat _colorAttachmentformat;

    OldPipelineBuilder() { clear(); }

public:

    void clear();
    void set_input_topology(VkPrimitiveTopology topology);
    void set_polygon_mode(VkPolygonMode mode);
    void set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace);
    void set_multisampling_none();
    void disable_blending();
    void enable_blending_additive();
    void enable_blending_alphablend();
    void set_color_attachment_format(VkFormat format);
    void set_depth_format(VkFormat format);
    void disable_depthtest();
    void enable_depthtest(bool depthWriteEnable, VkCompareOp op);


    VkPipeline build_pipeline(VkDevice device, VkRenderPass renderPass, const VkPipelineVertexInputStateCreateInfo& _vertexInputInfo);

    void set_shaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);



};




