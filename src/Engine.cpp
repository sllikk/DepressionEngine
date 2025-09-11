#include <iostream>
#include "Engine.h"


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

}

void Engine::run()
{
	while (!glfwWindowShouldClose(_window.get_window_ptr())) {

		glfwPollEvents();
	
	}
}

void Engine::cleanup()
{
	_window.destroy();
}





int main() {

	Engine engine;

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
