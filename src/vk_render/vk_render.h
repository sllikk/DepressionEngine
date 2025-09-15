#pragma once

#include "../IRender.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <vector>
#include <deque>
#include <array>
#include <functional>
#include <vma/vk_mem_alloc.h>


struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); //call functors
		}

		deletors.clear();
	}
};


struct FrameData {
	
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _renderSemaphore;
	VkSemaphore _swapchainSemaphore;
	VkFence _renderFence;

};

// triple buffering
constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanRender : public IRender {

	GLFWwindow* _window = nullptr;
	bool blsEngineInit = false;
	VkExtent2D _windowExtent{};
	int frame = 0; // current frames
	std::array<FrameData, FRAME_OVERLAP> _frames;

	FrameData& get_current_frame() { return _frames[frame % FRAME_OVERLAP]; }


	// enable validation layers for vulkan if debug, but disable if release 
#if NDEBUG
	bool enable_validation_layers = false;
#else
	bool enable_validation_layers = true;
#endif

	VkInstance _instance = VK_NULL_HANDLE;
	VkPhysicalDevice _chosenGPU = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
	VkSurfaceKHR _surface = VK_NULL_HANDLE;
	VkQueue _graphicsQueue = VK_NULL_HANDLE;
	uint32_t _graphicsQueueFamily = NULL;

	VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
	VkExtent2D _swapchainExtent{};
	VkFormat _swapchainImageFormat = VK_FORMAT_UNDEFINED;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	uint32_t swapchainImageIndex = 0;
	



public:

	// Inherited via IRender
	void init(int width, int height, void* ptr_window) override;
	void cleanup() override;
	void beginRenderFrame() override;
	void redner() override;
	void endRenderFrame() override;

private:

	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();


	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();



};