#include "CompShaderLoader.h"

CompShaderLoader::CompShaderLoader(const char* computePath)
{
	
	// 1. retrieve the compute source code from filePath
	std::string computeCode;
	std::ifstream cShaderFile;

	// ensure ifstream objects can throw exceptions:
	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file
		cShaderFile.open(computePath);
		std::stringstream cShaderStream;
		// read file's buffer contents into streams
		cShaderStream << cShaderFile.rdbuf();
		// close file handlers
		cShaderFile.close();
		// convert stream into string
		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		std::cout << "VERTEX_SHADER File path: " << computePath << std::endl;
	}
	const char* cShaderCode = computeCode.c_str();

	// 2. Compile and link shader
	unsigned int compute;
	// compute shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	// checking that the shader code compilation was successful
	if (!checkCompileErrors(compute, "SHADER")) exit(-1);
	// creating the final shader program
	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);
	// checking for shader linking errors
	if (!checkCompileErrors(ID, "PROGRAM")) exit(-1);
	// since we have compiled a complete shader program, we no longer need the stand allone vertex and fragment shaders
	glDeleteShader(compute);
	
};

CompShaderLoader::~CompShaderLoader()
{
};


void CompShaderLoader::useShaderStorageBuffer(GLsizeiptr size, void* data) 
{
	glGenBuffers(1, &BuffObjNameArry);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, BuffObjNameArry);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, BuffObjNameArry);
	unbindShaderStorageBuffer();
}

void CompShaderLoader::bindShaderStorageBuffer()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, BuffObjNameArry);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, BuffObjNameArry);
}

void CompShaderLoader::unbindShaderStorageBuffer()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}