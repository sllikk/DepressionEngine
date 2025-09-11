#pragma once 

#include <GLFW/glfw3.h>
#include <string>
#include <memory>

struct Window {

private:
	
	const std::string& engine_name = "DepressionEngine";
	const int width = 1280;
	const int height = 960;
	GLFWwindow* _window = nullptr;

public:

	void create();
	void destroy();

	GLFWwindow* get_window_ptr() const { return _window; }
	



};

class Engine {

	Window _window;

public: 

	void init();
	void run();
	void cleanup();



};
