#include "ShaderLoader.h"


ShaderLoader::ShaderLoader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		std::cout << "VERTEX_SHADER File path: " << vertexPath << std::endl;
		std::cout << "FRAGMENT_SHADER File path: " << fragmentPath << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Compile and link shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// checking that the shader code compilation was successful
	if (!checkCompileErrors(vertex, "SHADER")) exit(-1);
	// fragment 
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// checking that the compilation was successful
	if (!checkCompileErrors(fragment, "SHADER")) exit(-1);
	// creating the final shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// checking for shader linking errors
	if (!checkCompileErrors(ID, "PROGRAM")) exit(-1);
	// since we have compiled a complete shader program, we no longer need the stand allone vertex and fragment shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
};

void ShaderLoader::useUniformBuffer(GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_UNIFORM_BUFFER, ID);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, ID, 0, size);
}


// Set uniform functions
void ShaderLoader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
};

void ShaderLoader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};

void ShaderLoader::setFloat(const std::string& name, float value) const
{
	/*
	This function call below results in GL_INVALID_VALUE glerror.

	The cause as stated in the docs: https://docs.gl/gl4/glUniform
		"GL_INVALID_VALUE is generated if count is less than 0."
	However, this function does not have a cout parameter? :
		"void glUniform1f( GLint location, GLfloat v0 );"

	Am very confused, potential reasonings:
	 - value is an extreamly low float (0.00XX) which could flag that parameter s invalid?
	 - glGetUniformLocation() is returning something funky?
	*/
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	// Uncomment line below to print error 
	//std::cout << glGetError() << std::endl;
};

void ShaderLoader::setVec4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
};

void ShaderLoader::setUniformBufferSubData(GLintptr offset, GLsizeiptr size, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, ID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

bool ShaderLoader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
	}
	return true;
}