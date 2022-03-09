#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <stdio.h>
//#include <iostream>
//#include <cstdlib>

#include "ShaderLoader.h"
#include "CompShaderLoader.h"
#include "Window_GLFW.h"
#include "Agent.cpp"

// define
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "Physarum Simulation."
#define VERTEX_SHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\VertexShader.vert.shader"
#define FRAGMENT_SHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\FragmentShader.frag.shader"
#define AGENTMOVMENT_COMPUTESHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\AgentMovment.comp.shader"
#define TRAILMAP_COMPUTESHADER_FILE_LOCATION "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\TrailMap.comp.shader"

/*
	Due to efficency reasons, the minimum number of agents is 64. This is because work groups are 
	allocated in blocks divisable by 64 to improve the efficency and maximum count of aganets. Any
	less than 64 agents and no agents will be rendered, as the dispached workgroup size will be 0.
*/
const size_t NUMBER_OF_AGENTS = 64;

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
	const char* agentMovmentShaderFile = AGENTMOVMENT_COMPUTESHADER_FILE_LOCATION;
	const char* trailMapShaderFile = TRAILMAP_COMPUTESHADER_FILE_LOCATION;
	ShaderLoader GeometryShaderProg(vertexShaderFile, fragmentShaderFile);
	CompShaderLoader AgentMovmentProg(agentMovmentShaderFile);
	CompShaderLoader TrailMapProg(trailMapShaderFile);

	// set up compute shader storage buffers
	AgentSim agentSim = AgentSim(WINDOW_WIDTH, WINDOW_HEIGHT, NUMBER_OF_AGENTS);
	AgentMovmentProg.useShaderStorageBuffer(NUMBER_OF_AGENTS * sizeof(Agent), (void*)&agentSim.getAgents()[0]);

	// create an input and output texture to read and write to
	unsigned int inp_TextureID, out_TextureID;
	// input texture
	glGenTextures(1, &inp_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inp_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, inp_TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	// output texture
	glGenTextures(1, &out_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, out_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, out_TextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// setup buffers for the texture we render
	unsigned int tex_VAO, tex_VBO;
	glGenBuffers(1, &tex_VBO);
	glGenVertexArrays(1, &tex_VAO);
	glBindVertexArray(tex_VAO);

	float textureVerts[6][4] =
	{//  u      v     tex_x, tex_y
		 1.0f,  1.0f, 1.0f,  1.0f,
		 1.0f, -1.0f, 1.0f,  0.0f,
		-1.0f,  1.0f, 0.0f,  1.0f,
		 1.0f, -1.0f, 1.0f,  0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f,
		-1.0f,  1.0f, 0.0f,  1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureVerts), &textureVerts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(tex_VAO);

	GLuint groups_agent = NUMBER_OF_AGENTS / 64;
	GLuint groups_x = WINDOW_WIDTH / 8;
	GLuint groups_y = WINDOW_HEIGHT / 8;

	// -------------------- main render loop --------------------
	while (!window.windowShouldClose())
	{
		// input
		window.active();
		float deltaTime = window.getDeltaTime();

		// Update agent movement
		AgentMovmentProg.use();
		AgentMovmentProg.setFloat("deltaTime", deltaTime);
		glDispatchCompute(groups_agent, 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

		// update trail map
		TrailMapProg.use();
		glDispatchCompute(groups_x, groups_y, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		// render Geometry (texture)
		// ----------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// switch to vert and frag shader and draw texture quad
		GeometryShaderProg.use();
		glBindTexture(GL_TEXTURE_2D, out_TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// events
		// ----------
		window.nextRender();

	}

	// de-allocate all resources once they've outlived their purpose:
	// --------------------------------------------------------------
	glDeleteBuffers(1, &tex_VBO);
	glDeleteVertexArrays(1, &tex_VAO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


