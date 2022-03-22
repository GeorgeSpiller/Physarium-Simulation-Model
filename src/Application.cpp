#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <stdio.h>
//#include <iostream>
//#include <cstdlib>

#include "ShaderLoader.h"
#include "CompShaderLoader.h"
#include "Window_GLFW.h"
#include "Agent.h"
#include "PrePatterning.h"


// consts
constexpr auto WINDOW_NAME = "Physarum Simulation.";
constexpr auto VERTEX_SHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\VertexShader.vert.shader";
constexpr auto FRAGMENT_SHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\FragmentShader.frag.shader";
constexpr auto AGENTMOVMENT_COMPUTESHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\AgentMovment.comp.shader";
constexpr auto TRAILMAP_COMPUTESHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\TrailMap.comp.shader";

/*
	Due to efficency reasons, the minimum number of agents is 64 and needs to be a multiple of 64.
	This is because work groups are allocated in blocks divisable by 64 to improve the efficency
	and maximum count of aganets. Any less than 64 agents and no agents will be rendered, as the
	dispached workgroup size will be 0.
	Eg: const size_t NUMBER_OF_AGENTS = 64;
*/
// ---------- simulation settings: Poster background effects ---------- 
constexpr auto WINDOW_IS_FULLSCREEN = false;
constexpr auto WINDOW_WIDTH = 1782;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
constexpr auto WINDOW_HEIGHT = 960;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
constexpr auto TRAILMAP_trailDiffuseSpeed = 1.0f;		// higher value = shorter trail
constexpr auto TRAILMAP_trailEvaporationSpeed = 1.0f;	// higher value = trails evaporate faster
constexpr auto AGENT_movmentSpeed = 40.0f;
constexpr auto AGENT_turnSpeed = 50.0f;
constexpr auto AGENT_sensorOffsetDst = 6.0f;			// how far away the sensors (F) are from agent
constexpr auto AGENT_sensorAngleSpacing = AGENT_turnSpeed;// FL and FR sensor angle difference from F sensor
constexpr auto AGENT_sensorSize = 3.0f;					// size of sesor samplying area
size_t NUMBER_OF_AGENTS = 80000; // always multiples of 64: 50048, 60032, 70016, 80000, 90048, 100032, ..., 499008

/*

// ---------- simulation settings:  Experimetation, good growth behaviours----------
constexpr auto WINDOW_IS_FULLSCREEN = false;
constexpr auto WINDOW_WIDTH = 1830;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
constexpr auto WINDOW_HEIGHT = 960;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
constexpr auto TRAILMAP_trailDiffuseSpeed = 1.0f;		// higher value = shorter trail
constexpr auto TRAILMAP_trailEvaporationSpeed = 1.0f;	// higher value = trails evaporate faster
constexpr auto AGENT_movmentSpeed = 40.0f;
constexpr auto AGENT_turnSpeed = 50.0f;
constexpr auto AGENT_sensorOffsetDst = 6.0f;			// how far away the sensors (F) are from agent
constexpr auto AGENT_sensorAngleSpacing = AGENT_turnSpeed;// FL and FR sensor angle difference from F sensor
constexpr auto AGENT_sensorSize = 3.0f;					// size of sesor samplying area
size_t NUMBER_OF_AGENTS = 80000; // always multiples of 64: 50048, 60032, 70016, 80000, 90048, 100032, ..., 499008


//// ---------- simulation settings: First ever network!!! ---------- 
//constexpr auto WINDOW_IS_FULLSCREEN = false;
//constexpr auto WINDOW_WIDTH = 1200;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto WINDOW_HEIGHT = 800;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto TRAILMAP_trailDiffuseSpeed = 1.0f;		// higher value = shorter trail
//constexpr auto TRAILMAP_trailEvaporationSpeed = 1.0f;	// higher value = trails evaporate faster
//constexpr auto AGENT_movmentSpeed = 40.0f;
//constexpr auto AGENT_turnSpeed = 50.0f;
//constexpr auto AGENT_sensorOffsetDst = 6.0f;			// how far away the sensors (F) are from agent
//constexpr auto AGENT_sensorAngleSpacing = AGENT_turnSpeed;			// FL and FR sensor angle difference from F sensor
//constexpr auto AGENT_sensorSize = 3.0f;					// size of sesor samplying area
//const size_t NUMBER_OF_AGENTS = 80000;


//// ---------- simulation settings: some following behaviour ---------- 
//constexpr auto WINDOW_IS_FULLSCREEN = false;
//constexpr auto WINDOW_WIDTH = 1200;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto WINDOW_HEIGHT = 800;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto TRAILMAP_trailDiffuseSpeed = 20.0f;		// higher value = shorter trail
//constexpr auto TRAILMAP_trailEvaporationSpeed = 1.0f;	// higher value = trails evaporate faster
//constexpr auto AGENT_movmentSpeed = 50.0f;
//constexpr auto AGENT_turnSpeed = 3.1f;
//constexpr auto AGENT_sensorOffsetDst = 6.0f;			// how far away the sensors (F) are from agent
//constexpr auto AGENT_sensorAngleSpacing = AGENT_turnSpeed;			// FL and FR sensor angle difference from F sensor
//constexpr auto AGENT_sensorSize = 3.0f;					// size of sesor samplying area
//const size_t NUMBER_OF_AGENTS = 80000;


//// ---------- simulation settings: CharacteristicsOfPhysarum.pdf ---------- 
//constexpr auto WINDOW_IS_FULLSCREEN = false;
//constexpr auto WINDOW_WIDTH = 1024;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto WINDOW_HEIGHT = 576;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto TRAILMAP_trailDiffuseSpeed = 50.0f;		// higher value = shorter trail
//constexpr auto TRAILMAP_trailEvaporationSpeed = 0.9f;	// higher value = trails evaporate faster
//constexpr auto AGENT_movmentSpeed = 10.0f;
//constexpr auto AGENT_turnSpeed = 45.0f;
//constexpr auto AGENT_sensorOffsetDst = 9.0f;
//constexpr auto AGENT_sensorAngleSpacing = 22.5f;
//constexpr auto AGENT_sensorSize = 1.0f;
//const size_t NUMBER_OF_AGENTS = 35389;


//// ---------- simulation settings: GitHub Pysarum Sim ---------- 
//constexpr auto WINDOW_IS_FULLSCREEN = true;
//constexpr auto WINDOW_WIDTH = 1920;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto WINDOW_HEIGHT = 1080;			// if WINDOW_IS_FULLSCREEN is set, these are ignored
//constexpr auto TRAILMAP_trailDiffuseSpeed = 20.0f;
//constexpr auto TRAILMAP_trailEvaporationSpeed = 0.9f;
//constexpr auto AGENT_movmentSpeed = 50.0f;
//constexpr auto AGENT_turnSpeed = 100.0f;
//constexpr auto AGENT_sensorOffsetDst = 16.0f;	
//constexpr auto AGENT_sensorAngleSpacing = 0.6f;
//constexpr auto AGENT_sensorSize = 3.0f;
//const size_t NUMBER_OF_AGENTS = 131072;

*/

/*
	This code was adapted using the following resources:
	
	tutorial by Joey de Vries:
	https://learnopengl.com/Getting-started/Shaders
	https://twitter.com/JoeyDeVriez

	Usful repo of Physarum model based in unity:
	https://github.com/SebLague/Slime-Simulation

	OpenGL SuperBible (G. Sellers, R. S. Wright, N. Haemel):
	Compute Shaders intro: p467
	Compute Shader Flocking Behaviour: p492

*/


int main()
{

	srand((unsigned)(time(NULL)));

	Window_GLFW window = Window_GLFW(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_IS_FULLSCREEN, WINDOW_NAME, 0); // custom size
	// Window_GLFW window = Window_GLFW(WINDOW_NAME, 0); // fullscreen (1920 x 1080);

	AgentSim agentSim = AgentSim(window.getWidth(), window.getHeight(), NUMBER_OF_AGENTS, SpawnMode::RECT);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL   Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	const char* vertexShaderFile = VERTEX_SHADER_FILE_LOCATION;
	const char* fragmentShaderFile = FRAGMENT_SHADER_FILE_LOCATION;
	const char* agentMovmentShaderFile = AGENTMOVMENT_COMPUTESHADER_FILE_LOCATION;
	const char* trailMapShaderFile = TRAILMAP_COMPUTESHADER_FILE_LOCATION;
	
	ShaderLoader GeometryShaderProg(vertexShaderFile, fragmentShaderFile);
	CompShaderLoader AgentMovmentProg(agentMovmentShaderFile);
	CompShaderLoader TrailMapProg(trailMapShaderFile);

	// set up compute shader storage buffers
	AgentMovmentProg.useShaderStorageBuffer(NUMBER_OF_AGENTS * sizeof(Agent), (void*)&agentSim.getAgents()[0]);

	// create an input and output texture to read/write and write only to, respectivly
	unsigned int inp_TextureID, out_TextureID;
	// input texture
	glGenTextures(1, &inp_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inp_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.getWidth(), window.getHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, inp_TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	// output texture
	glGenTextures(1, &out_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, out_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window.getWidth(), window.getHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, out_TextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	// setup buffers for the texture we render
	// ------------------------------------------------------------
	// believe the movment issue may involve the seting up of the buffers bellow
	// - Where does it read output from comput shader to render agent positions?
	// - Does this geometry get textured by the trail map compute shader?
	// - If not, where does it get shaded?
	// - What gets passed to the vert and frag shaders?
	// - Where can i access the output of the AgentMovment and TrailMap Compute?
	// ------------------------------------------------------------
	GLuint tex_VAO, tex_VBO = 0;
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

	// set uniform settings (that are constant) for agentMovment compute and trailMap compute
	AgentMovmentProg.use();
	AgentMovmentProg.setFloat("agentMovmentSpeed", AGENT_movmentSpeed);
	AgentMovmentProg.setFloat("agentTurnSpeed", AGENT_turnSpeed);
	AgentMovmentProg.setVec3("agentSensorSettings", AGENT_sensorOffsetDst, AGENT_sensorAngleSpacing, AGENT_sensorSize);

	TrailMapProg.use();
	TrailMapProg.setFloat("trailDiffuseSpeed", TRAILMAP_trailDiffuseSpeed);
	TrailMapProg.setFloat("trailEvaporationSpeed", TRAILMAP_trailEvaporationSpeed);

	GLuint groups_agent = NUMBER_OF_AGENTS / 64;
	GLuint groups_x = window.getWidth() / 8;
	GLuint groups_y = window.getHeight() / 8;

	float DEBUG_uniform = 0.0;
	float deltaT = 0.0f;
	// ------------------------------------------------------------
	// - Do we strictly need to bind and unbind the ShaderStorageBuffer each loop?
	// - Is this where we can interact with gl image buffers? 
	// - Where does the TrailMap compute write to? Where is outImg stored/refferenced to here?
	// - Can we replace the vert array of the texture quad to render using index array instead?
	// - How does the compute shader know where out out_TextureID is?
	// ------------------------------------------------------------
	// -------------------- main render loop --------------------
	while (!window.windowShouldClose())
	{
		// input
		window.active();
		deltaT = window.getDeltaTime();
		
		// std::cout << "\r" << "DeltaT: " << deltaT << std::flush;

		// debug_newAngle += 0.1f * deltaT;
		// if (debug_newAngle > TWO_PI) { debug_newAngle = 0.0f; }
		// Update agent movement
		AgentMovmentProg.use();
		AgentMovmentProg.setFloat("deltaTime", deltaT);
		AgentMovmentProg.setFloat("randomSeed", static_cast <float> (rand()));

		glDispatchCompute(groups_agent, 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
		glFinish();

		// update trail map
		TrailMapProg.use();
		TrailMapProg.setFloat("deltaTime", deltaT);
		glDispatchCompute(groups_x, groups_y, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glFinish();

		// render Geometry (texture)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	std::cout << "GL error stack: " << glGetError() << std::endl;
	
	// de-allocate all resources once they've outlived their purpose:
	// --------------------------------------------------------------
	glDeleteBuffers(1, &tex_VBO);
	glDeleteVertexArrays(1, &tex_VAO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


