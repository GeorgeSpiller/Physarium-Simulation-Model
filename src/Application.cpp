#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <stdio.h>
//#include <iostream>
//#include <cstdlib>

#include "ShaderLoader.h"
#include "CompShaderLoader.h"
#include "Window_GLFW.h"
#include "Agent.h"
#include "LoadPrePattern.h"
#include "stb_image.h"


// consts
constexpr auto WINDOW_NAME = "Physarum Simulation.";
constexpr auto VERTEX_SHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\VertexShader.vert.shader";
constexpr auto FRAGMENT_SHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\FragmentShader.frag.shader";
constexpr auto AGENTMOVMENT_COMPUTESHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\AgentMovment.comp.shader";
constexpr auto TRAILMAP_COMPUTESHADER_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\Shaders\\TrailMap.comp.shader";
constexpr auto PREPATTERN_IMAGE_FILE_LOCATION = "D:\\Users\\geosp\\Documents\\__Work\\.Uni\\FinalYear\\Diss\\PhysarumSimulation\\PhysarumSimulation\\src\\PrePatternImages\\DiffuseNodes.png"; // DiffuseNodes.png  SmallNodeGraphComplex.png


/*
	Due to efficency reasons, the minimum number of agents is 64 and needs to be a multiple of 64.
	This is because work groups are allocated in blocks divisable by 64 to improve the efficency
	and maximum count of aganets. Any less than 64 agents and no agents will be rendered, as the
	dispached workgroup size will be 0.
	Eg: const size_t NUMBER_OF_AGENTS = 64;
*/
// ---------- simulation settings: prepatterning with large diffuese nodes ---------- 
constexpr auto WINDOW_IS_FULLSCREEN = false;
constexpr auto WINDOW_WIDTH = 1784;							// if WINDOW_IS_FULLSCREEN is set, these are ignored
constexpr auto WINDOW_HEIGHT = 960;							// if WINDOW_IS_FULLSCREEN is set, these are ignored
constexpr auto TRAILMAP_trailDiffuseSpeed = 1.5f;			// higher value = shorter trail
constexpr auto TRAILMAP_trailEvaporationSpeed = 0.8f;		// higher value = trails evaporate faster
constexpr auto AGENT_movmentSpeed = 40.0f;					// how fast the agents move each frame (NOT how fast the simulation runs)
constexpr auto AGENT_turnSpeed = 50.0f;						// turn speed in degrees
constexpr auto AGENT_sensorOffsetDst = 6.0f;				// how far away the sensors (F) are from agent
constexpr auto AGENT_sensorAngleSpacing = AGENT_turnSpeed;	// FL and FR sensor angle difference from F sensor
constexpr auto AGENT_sensorSize = 6.0f;						// size of sesor samplying area
GLuint NUMBER_OF_AGENTS = 50048; // always multiples of 64: 50048, 60032, 70016, 80000, 90048, 100032, ..., 499008

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

void paramCheck() 
{
	const char* paramCheckMessage = "---------- Simulation Parameter Warning ----------\nThe following parameters contain some descrepencies:\n";
	if (NUMBER_OF_AGENTS % 64 != 0) {
		std::cout << paramCheckMessage << "NUMBER_OF_AGENTS\n\t" <<
			"The number of agents MUST be a multiple of 64." << std::endl;
		exit(0);
	}
	if (WINDOW_IS_FULLSCREEN)
		std::cout << paramCheckMessage << "WINDOW_IS_FULLSCREEN\n\t" <<
		"Fullscreen functionality has not been fully implemented yet. Setting this will set the screen size fo 1920x1080 and may have undifined behavior." << std::endl;
	if (WINDOW_WIDTH % 8 != 0)
		std::cout << paramCheckMessage << "WINDOW_WIDTH\n\t" <<
		"Desired window width is not a multiple of 8. This can result in undifined behavour, most likely slower performance and erronious visual artifacts." << std::endl;
	if (WINDOW_HEIGHT % 8 != 0)
		std::cout << paramCheckMessage << "WINDOW_HEIGHT\n\t" <<
		"Desired window height is not a multiple of 8. This can result in undifined behavour, most likely slower performance and erronious visual artifacts." << std::endl;
	std::cout << "--------------------------------------------------" << std::endl << std::endl;
}

int main()
{
	srand((unsigned)(time(NULL)));
	paramCheck();

	// Create the window using GLFW. Fullscreen currently not fully implemented
	Window_GLFW window = Window_GLFW(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_IS_FULLSCREEN, WINDOW_NAME, 0);
	// Window_GLFW window = Window_GLFW(WINDOW_NAME, 0); // fullscreen (1920 x 1080);

	// Print GL and GLSL version information. Has to be done after creating window, as we need to have the glad headers loaded
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL   Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// SpawnModes: RANDOM, CIRCLE, POINT, POINT2, POINT4, RECT
	AgentSim agentSim = AgentSim(window.getWidth(), window.getHeight(), NUMBER_OF_AGENTS, SpawnMode::RANDOM);

	// Set up shader files
	const char* vertexShaderFile = VERTEX_SHADER_FILE_LOCATION;
	const char* fragmentShaderFile = FRAGMENT_SHADER_FILE_LOCATION;
	const char* agentMovmentShaderFile = AGENTMOVMENT_COMPUTESHADER_FILE_LOCATION;
	const char* trailMapShaderFile = TRAILMAP_COMPUTESHADER_FILE_LOCATION;
	ShaderLoader GeometryShaderProg(vertexShaderFile, fragmentShaderFile);
	CompShaderLoader AgentMovmentProg(agentMovmentShaderFile);
	CompShaderLoader TrailMapProg(trailMapShaderFile);

	// set up compute shader storage buffers
	AgentMovmentProg.useShaderStorageBuffer(NUMBER_OF_AGENTS * sizeof(Agent), (void*)&agentSim.getAgents()[0]);

	// Create textures to: 
	// 1: render the prepattern (inp_TextureID) and 
	// 2: render the final sim (pre pattern + agents, out_TextureID)
	unsigned int inp_TextureID, out_TextureID;
	const char* prepatternFile = PREPATTERN_IMAGE_FILE_LOCATION;

	LoadPrePattern prePattern = LoadPrePattern(prepatternFile, &inp_TextureID);
	prePattern.initalizeTexture(window.getWidth(), window.getHeight());

	LoadPrePattern outputTexture = LoadPrePattern(&out_TextureID);
	outputTexture.initalizeTexture(window.getWidth(), window.getHeight());

	// setup buffers for the texture we render
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

	// Workgroup sizes for compute shader (groups_agent) and trail map (groups_x, groups_y)
	GLuint groups_agent = NUMBER_OF_AGENTS / 64;
	GLuint groups_x = window.getWidth() / 8;
	GLuint groups_y = window.getHeight() / 8;

	float deltaT = 0.0f;

	std::cout << std::endl << " ---------- Entering Render Loop ---------- " << std::endl;
	// -------------------- main render loop --------------------
	while (!window.windowShouldClose())
	{
		// input
		window.active();
		deltaT = window.getDeltaTime();
		
		std::cout << "\r" << "DeltaT: " << deltaT << std::flush;

		// Update agent movement
		AgentMovmentProg.use();
		AgentMovmentProg.setFloat("deltaTime", deltaT);

		glDispatchCompute(groups_agent, 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
		glFinish();

		// update trail map
		TrailMapProg.use();
		TrailMapProg.setFloat("deltaTime", deltaT);
		glDispatchCompute(groups_x, groups_y, 1);
		// glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		// glFinish();

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
	std::cout << std::endl;
	std::cout << " ---------- End Render Loop ---------- " << std::endl;
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


