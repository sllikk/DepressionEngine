#include <iostream>
#include "Engine.h"
#include "vk_render/vk_render.h"

void Window::create()
{
	if(glfwInit() == false) {

		std::cerr << "Fail init glfw" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_window = glfwCreateWindow(width, height, engine_name.c_str(), nullptr, nullptr);
	if (_window == nullptr)
	{
		std::cerr << "Failed create window" << std::endl;
		return;
	}

}

void Window::destroy()
{
	glfwTerminate();
	glfwDestroyWindow(_window);
	

}
	

	
void Engine::init()
{
	_window.create();
	_render->init(_window.getWidth(), _window.getHeight(), _window.get_window_ptr());

}

void Engine::run()
{
	while (_window.blsShouldCloseWindow() == false) {

		glfwPollEvents();
	
		_render->beginRenderFrame();
		_render->redner();
		_render->endRenderFrame();

	}
}

void Engine::cleanup()
{
	_render->cleanup();
	_window.destroy();

}

Engine::Engine(std::unique_ptr<IRender> render) : _render(std::move(render))
{	

}





int main() {

	std::unique_ptr<VulkanRender> vk_render = std::make_unique<VulkanRender>();

	Engine engine(std::move(vk_render));

	try
	{
		engine.init();
		engine.run();
		engine.cleanup();

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}


	return 0;
};
