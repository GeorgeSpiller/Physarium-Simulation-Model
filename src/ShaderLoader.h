#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all thre required opengl hedders

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderLoader
{
public:
	// the program ID
	unsigned int ID;

	ShaderLoader(unsigned int ID = 0) : ID(ID) {}
	ShaderLoader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	void use() { glUseProgram(ID); };
	
	void useUniformBuffer(GLsizeiptr size);

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const;

	void setUniformBufferSubData(GLintptr offset, GLsizeiptr size, void* data);

protected:

	bool checkCompileErrors(unsigned int shader, std::string type);

};
#endif
