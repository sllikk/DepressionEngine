#pragma once
#include <vulkan/vulkan.h>

namespace vkinit
{
    VkSemaphoreCreateInfo semaphore_create_info(VkSemaphoreCreateFlags flags /*= 0*/);
    VkFenceCreateInfo fence_create_info(VkFenceCreateFlags flags /*= 0*/);

    VkSemaphoreSubmitInfo semaphore_submit_info(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
    VkSubmitInfo2 submit_info(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo, VkSemaphoreSubmitInfo* waitSemaphoreInfo);

    VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags /*= 0*/);
    VkCommandBufferAllocateInfo command_buffer_allocate_info( VkCommandPool pool, uint32_t count /*= 1*/);    
    VkCommandBufferBeginInfo command_buffer_begin_info(VkCommandBufferUsageFlags flags /*= 0*/);
    VkCommandBufferSubmitInfo command_buffer_submit_info(VkCommandBuffer cmd);


    VkImageSubresourceRange image_subresource_range(VkImageAspectFlags aspectMask /*= 0*/);
    VkImageCreateInfo image_create_info(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo image_view_create_info(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

    // FOR RENDER_PASS
    VkRenderPassBeginInfo renderpass_begin_info(VkRenderPass renderPass, VkExtent2D window_extent, VkFramebuffer framebuffer, VkClearValue* clearValue, uint32_t clearValueCount);

    // FOR DYNAMIC RENDERER TODO
    VkRenderingAttachmentInfo depth_attachment_info(VkImageView view, VkImageLayout layout);
    VkRenderingAttachmentInfo attachment_info(VkImageView view, VkClearValue* clear, VkImageLayout layout);
    VkRenderingInfo rendering_info(VkExtent2D swapchainExtent, VkRenderingAttachmentInfo* colorAttachment, VkRenderingAttachmentInfo* depthAttachment);

    // graphics pipeline
    VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule, const char* entry);
    VkPipelineLayoutCreateInfo pipeline_layout_create_info();
    
    VkPresentInfoKHR present_info();

   



};




