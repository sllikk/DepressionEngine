#define GLFW_INCLUDE_VULKAN


#include "vk_render.h"


#include "vk_initializers.h"
#include "vk_pipelines.h"

#include <iostream>
#include <VkBootstrap.h>


void VulkanRender::init(int width, int height, void* ptr_window)
{
	_window = static_cast<GLFWwindow*>(ptr_window);
	_windowExtent.width = width;
	_windowExtent.height = height;

	if (_window == nullptr) {
		
		std::cerr << "Window nullptr" << std::endl;
		return;
	}

	init_vulkan();
	init_swapchain();
	init_commands();
	init_sync_strucure();

	blsEngineInit = true;


}


void VulkanRender::cleanup()
{
	if (blsEngineInit = true)
	{
		//make sure the gpu has stopped doing its things
		vkDeviceWaitIdle(_device);

		// destroy all command pool
		for (int i = 0; i < FRAME_OVERLAP; i++) {	
			vkDestroyCommandPool(_device, _frames[i]._commandPool, nullptr);
		
			// destroy sync objects 
			vkDestroyFence(_device, _frames[i]._renderFence, nullptr);
			vkDestroySemaphore(_device, _frames[i]._renderSemaphore, nullptr);
			vkDestroySemaphore(_device, _frames[i]._swapchainSemaphore, nullptr);

		}

		destroy_swapchain();

		vkDestroySurfaceKHR(_instance, _surface, nullptr);
		vkDestroyDevice(_device, nullptr);

		vkb::destroy_debug_utils_messenger(_instance, _debugMessenger);
		vkDestroyInstance(_instance, nullptr);
		glfwDestroyWindow(_window);
	}

}


void VulkanRender::beginRenderFrame()
{
	//> draw_1
		// wait until the gpu has finished rendering the last frame. Timeout of 1
		// second
	VK_CHECK(vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, 1000000000));
	VK_CHECK(vkResetFences(_device, 1, &get_current_frame()._renderFence));
	//< draw_1

	VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, 1000000000, get_current_frame()._swapchainSemaphore, nullptr, &swapchainImageIndex));

	VkCommandBuffer cmd = get_current_frame()._mainCommandBuffer;

	VK_CHECK(vkResetCommandBuffer(cmd, 0));

	VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	//start the command buffer recording
	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

		//make the swapchain image into writeable mode before rendering
	vkutil::transition_image(cmd, _swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

	//make a clear-color from frame number. This will flash with a 120 frame period.
	VkClearColorValue clearValue;
	float flash = std::abs(std::sin(frame / 120.f));
	clearValue = { { 0.0f, 1.0f, flash, 1.0f } };

	VkImageSubresourceRange clearRange = vkinit::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);

	//clear image
	vkCmdClearColorImage(cmd, _swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);

	//make the swapchain image into presentable mode
	vkutil::transition_image(cmd, _swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	//finalize the command buffer (we can no longer add commands, but it can now be executed)
	VK_CHECK(vkEndCommandBuffer(cmd));
	//< draw_4

	//> draw_5
		//prepare the submission to the queue. 
		//we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
		//we will signal the _renderSemaphore, to signal that rendering has finished

	VkCommandBufferSubmitInfo cmdinfo = vkinit::command_buffer_submit_info(cmd);

	VkSemaphoreSubmitInfo waitInfo = vkinit::semaphore_submit_info(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, get_current_frame()._swapchainSemaphore);
	VkSemaphoreSubmitInfo signalInfo = vkinit::semaphore_submit_info(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, get_current_frame()._renderSemaphore);

	VkSubmitInfo2 submit = vkinit::submit_info(&cmdinfo, &signalInfo, &waitInfo);

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	VK_CHECK(vkQueueSubmit2(_graphicsQueue, 1, &submit, get_current_frame()._renderFence));
	//< draw_5
	// 
	//> draw_6
		//prepare present
		// this will put the image we just rendered to into the visible window.
		// we want to wait on the _renderSemaphore for that, 
		// as its necessary that drawing commands have finished before the image is displayed to the user
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.pSwapchains = &_swapchain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &get_current_frame()._renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &swapchainImageIndex;

	VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

	//increase the number of frames drawn
	frame++;


}


void VulkanRender::redner()
{

}


void VulkanRender::endRenderFrame()
{

}


/// <summary>
///  -------------------------------------------------------------------------------------VULKAN FUNCTION AND RENDER-----------------------------------------------------------------------------------------
/// </summary>

void VulkanRender::init_vulkan()
{
	vkb::InstanceBuilder insrance_builder;

	auto instance_ret = insrance_builder
		.set_engine_name("DepressionEngine")
		.require_api_version(1, 3, 0)
		.use_default_debug_messenger()
		.enable_validation_layers(enable_validation_layers)
		.build();

	
	_instance = instance_ret.value();
	_debugMessenger = instance_ret.value().debug_messenger;

	if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS)
	{
		std::cerr << "Failed create window surface" << std::endl;
	}


	//vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features.synchronization2 = true;
	features.dynamicRendering = true;

	//vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;

	vkb::PhysicalDeviceSelector selector{ instance_ret.value()};
	vkb::PhysicalDevice gpu = selector.set_minimum_version(1, 3)
		.set_required_features_12(features12)
		.set_required_features_13(features)
		.set_surface(_surface)
		.select()
		.value();

	vkb::DeviceBuilder deviceBuilder{ gpu };

	vkb::Device vkbDevice = deviceBuilder.build().value();
	

	// Get the VkDevice handle used in the rest of a vulkan application
	_device = vkbDevice.device;
	_physicalDevice = vkbDevice.physical_device;

	_graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	_graphicsQueueIndex = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

}


void VulkanRender::init_swapchain()
{
	create_swapchain(_windowExtent.width, _windowExtent.height);
}


void VulkanRender::init_commands()
{
	const VkCommandPoolCreateInfo& command_pool_info = vkinit::command_pool_create_info(_graphicsQueueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	for (int i = 0; i < FRAME_OVERLAP; i++) {

		VK_CHECK(vkCreateCommandPool(_device, &command_pool_info, nullptr, &_frames[i]._commandPool));

		const VkCommandBufferAllocateInfo& allocate_buffer = vkinit::command_buffer_allocate_info(_frames[i]._commandPool, 1);
		
		VK_CHECK(vkAllocateCommandBuffers(_device, &allocate_buffer, &_frames[i]._mainCommandBuffer));

	}


}


void VulkanRender::init_sync_strucure()
{	
	const VkFenceCreateInfo& fence_info = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
	const VkSemaphoreCreateInfo& semaphore_info = vkinit::semaphore_create_info(0);


	for (int i = 0; i < FRAME_OVERLAP; i++) {

		 VK_CHECK(vkCreateFence(_device, &fence_info, nullptr, &_frames[i]._renderFence));
		 VK_CHECK(vkCreateSemaphore(_device, &semaphore_info, nullptr, &_frames[i]._renderSemaphore));
		 VK_CHECK(vkCreateSemaphore(_device, &semaphore_info, nullptr, &_frames[i]._swapchainSemaphore));
	
	}

}



void VulkanRender::create_swapchain(int width, int height)
{
	vkb::SwapchainBuilder swapchainBuilder{ _physicalDevice, _device, _surface };
	
	_swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = _swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

	_swapchainExtent = vkbSwapchain.extent;
	//store swapchain and its related images
	_swapchain = vkbSwapchain.swapchain;
	_swapchainImages = vkbSwapchain.get_images().value();
	_swapchainImageViews = vkbSwapchain.get_image_views().value();

}

void VulkanRender::destroy_swapchain()
{
	vkDestroySwapchainKHR(_device, _swapchain, nullptr);

	for (int i = 0; i < _swapchainImageViews.size(); i++) {

		vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
	}
}

