#include "../IRender.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <vector>
#include <deque>
#include <array>

struct FrameData {
	
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
	VkSemaphore _renderSemaphore, _swapchainSemaphore;
	VkFence _renderFence;

};

// triple buffering
constexpr int FRAME_OVERLAP = 3;

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
	VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
	VkSurfaceKHR _surface = VK_NULL_HANDLE;
	VkQueue _graphycsQueue = VK_NULL_HANDLE;
	uint32_t _graphycsQueueIndex = NULL;

	VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
	VkExtent2D _swapchainExtent{};
	VkFormat _swapchainImageFormat = VK_FORMAT_UNDEFINED;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;

	

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
	void init_sync_strucure();

	void create_swapchain(int width, int height);
	void destroy_swapchain();

};