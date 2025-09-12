
#define GLFW_INCLUDE_VULKAN

#include "vk_render.h"

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

	//vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;
	features12.timelineSemaphore = true;

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

	_graphycsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	_graphycsQueueIndex = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

}


void VulkanRender::init_swapchain()
{
	create_swapchain(_windowExtent.width, _windowExtent.height);
}


void VulkanRender::init_commands()
{


}


void VulkanRender::init_sync_strucure()
{


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

