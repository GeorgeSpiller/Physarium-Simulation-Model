#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window_GLFW
{
public:
	Window_GLFW(int width, int height, bool isFullScreen, std::string title, int swapInterval);
	~Window_GLFW();

	void active();
	void nextRender();

	// register a callback function on the window to handle any resizing and movment operations
	int windowShouldClose() const { return glfwWindowShouldClose(w_win); }
	int getWidth() const { return w_winWidth; }
	int getHeight() const { return w_winHeight; }
	float getDeltaTime() const { return w_deltaTime; }

private:
	void processInput();

	GLFWwindow* w_win;

	int w_winWidth, w_winHeight;
	float w_deltaTime = 0.0f;
	float w_previousFrame = 0.0f;
};



