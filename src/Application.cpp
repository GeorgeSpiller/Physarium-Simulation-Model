#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <stdio.h>
//#include <iostream>
//#include <cstdlib>

// #include "Shader.h"
#include "CompShaderLoader.h"
#include "Window_GLFW.h"

// define
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "Physarum Simulation."
#define VERTEX_SHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\VertexShader.vert.shader"
#define FRAGMENT_SHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\FragmentShader.frag.shader"
#define COMPUTE_SHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\ComputeShader.comp.shader"

/*
	This code was adapted from the following tutorial by Joey de Vries:
	https://learnopengl.com/Getting-started/Shaders
	https://twitter.com/JoeyDeVriez

	Usful repo:
	https://github.com/riccardo-franchi/Physarum-Simulation

	OpenGL SuperBible (G. Sellers, R. S. Wright, N. Haemel):
	Compute Shaders intro: p467
	Compute Shader Flocking Behaviour: p492
*/

const int number_of_agents = 1;

int main()
{
	// srand((unsigned)(time(NULL)));

	Window_GLFW window = Window_GLFW(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, false, 0);

	const char* vertexShaderFile = VERTEX_SHADER_FILE_LOCATION;
	const char* fragmentShaderFile = FRAGMENT_SHADER_FILE_LOCATION;
	const char* computeShaderFile = COMPUTE_SHADER_FILE_LOCATION;
	ShaderLoader GeometryShader(vertexShaderFile, fragmentShaderFile);
	CompShaderLoader ComputeShader(computeShaderFile);

	// -------------------- main render loop --------------------
	while (!window.windowShouldClose())
	{
		// input
		// ----------
		window.active();

		// update using compute shader
		ComputeShader.use();
		glDispatchCompute(1, 1, 1); 

		// render
		// ----------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// switch to vert and frag shader
		GeometryShader.use();

		// render 

		// events
		// ----------
		window.nextRender();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


