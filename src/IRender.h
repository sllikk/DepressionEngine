#pragma once 


class IRender {

public:

	virtual void init(int width, int height, void* ptr_window) = 0;
	virtual void cleanup() = 0;


	virtual void beginRenderFrame() = 0;
	virtual void redner() = 0;
	virtual void endRenderFrame() = 0;

};



