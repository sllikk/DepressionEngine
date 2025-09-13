#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <functional>
#include <deque>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vma/vk_mem_alloc.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

/*----------------------------------------------------------------SCENE_GRAPH----------------------------------------------------------------*/

struct DrawContext;



struct Node
{
    std::weak_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;

    glm::mat4 worldTransform;
    glm::mat4 localTransform;

    void updateTransform(const glm::mat4& parentMatrix) {
        worldTransform = parentMatrix * localTransform;
        for ( const auto& c : children ) {
            c->updateTransform(worldTransform);
        }
    };

    
    virtual void Draw(const glm::mat4& topMatrix, DrawContext& ctx) {
        // draw for children 
        for (const auto& c : children) {
            c->Draw(topMatrix, ctx);
        }
    };

};


/*----------------------------------------------------------------SCENE_GRAPH_END----------------------------------------------------------------*/



/*----------------------------------------------------------------MATERIAL----------------------------------------------------------------*/

enum class MaterialPass : uint8_t {
    
    MAIN_COLOR,
    TRANSPARENT,
    OTHER

};

struct MaterialPipeline {
    VkPipeline pipeline;
    VkPipelineLayout layout;

};

struct MaterialInstance {

    MaterialPipeline* pipeline;
    VkDescriptorSet materialSet;
    MaterialPass passType;
};

/*----------------------------------------------------------------MATERIAL_END----------------------------------------------------------------*/

struct AllocatedImage
{
    VkImage image = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VkExtent3D imageExtent;
    VkFormat imageFormat;
};


struct AllocatedBuffer
{
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VmaAllocationInfo info;
};


// We use PVP+ Adress device
struct Vertex
{
    glm::vec3 pos;
    float uv_x;
    glm::vec3 normal;
    float uv_y;
    glm::vec4 color;

};

struct VertexBuffer {

public:
    
    glm::vec3 _pos;
    glm::vec4 _color;
    glm::vec2 _texCoord;
        
    static __forceinline VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescriptor{};
        bindingDescriptor.binding = 0;
        bindingDescriptor.stride = sizeof(Vertex);
        bindingDescriptor.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptor;
    };

    // ONLY FOR TEST TEXTURES THAT IS NOT USE IN APP!!!
    static __forceinline std::array<VkVertexInputAttributeDescription, 3> getVertexAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(VertexBuffer, _pos);
      
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(VertexBuffer, _color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(VertexBuffer, _texCoord);

        return attributeDescriptions;
    };

};



struct GPUMeshBuffers 
{
    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    VkDeviceAddress vertexAddressBuffer;
};

struct GPUPushConstant 
{
    glm::mat4 renderer_matrix;
    VkDeviceAddress vertexBuffer;

    
};

struct GPUSceneData {

    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 viewproj;
    glm::vec4 ambientColor;
    glm::vec4 sunlightDirection; //w for sun power
    glm::vec4 sunlightColor;
};


// FOR CATCH ERROR
#define VK_CHECK(x)                                                     \
    do {                                                                \
        VkResult err = x;                                               \
        if (err) {                                                      \
             printf_s("Detected Vulkan error: %s", string_VkResult(err)); \
            abort();                                                    \
        }                                                               \
    } while (0)