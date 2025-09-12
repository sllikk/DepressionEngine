#pragma once 

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include "IRender.h"

struct Window {

private:
	
	const std::string& engine_name = "DepressionEngine";
	int width = 1280;
	int height = 960;
	GLFWwindow* _window = nullptr;

public:

	void create();
	void destroy();

	inline GLFWwindow* get_window_ptr() const { return _window; }
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

	inline bool blsShouldCloseWindow() const { return glfwWindowShouldClose(_window); }

};

class Engine {

	Window _window;
	std::unique_ptr<IRender> _render;

public: 

	void init();
	void run();
	void cleanup();

	Engine(std::unique_ptr<IRender> render);
	Engine() = default;

};
