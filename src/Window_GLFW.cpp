#include "Window_GLFW.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// resizes the window
	glViewport(0, 0, width, height);
}

Window_GLFW::Window_GLFW(int width, int height, std::string title, bool fullscreen, int swapInterval)
{
	// initialise GLFW with some WindowHints to define version 4.4 (core profile)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// create a basic window and make it part of the current context
	GLFWwindow* w = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (w == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
	};
	glfwMakeContextCurrent(w);

	w_win = w;
	w_winWidth = width;
	w_winHeight = height;

	// Using GLAD to load openGL function deffinitions
	// using glfwGetProcAddress and casting return GLADloadproc type, then loading it with gladLoadGLLoader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		glfwTerminate();
	};

	glViewport(0, 0, width, height);
	GLFWframebuffersizefun callBackResize = framebuffer_size_callback;
	glfwSetFramebufferSizeCallback(w, callBackResize);
}

Window_GLFW::~Window_GLFW()
{
	glfwTerminate();
}

void Window_GLFW::active()
{
	float currFrame = (float)glfwGetTime();
	w_deltaTime = currFrame - w_previousFrame;
	w_previousFrame = currFrame;

	processInput();
}

void Window_GLFW::nextRender()
{
	glfwSwapBuffers(w_win);
	glfwPollEvents();
}

void Window_GLFW::processInput()
{
	if (glfwGetKey(w_win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(w_win, true);
	}
}