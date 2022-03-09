#pragma once
#ifndef COMPSHADER_H
#define COMPSHADER_H

#include <glad/glad.h> // include glad to get all thre required opengl hedders
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderLoader.h"


/*
	Effectivly the same code as Shader.h.
	Used as you cannot link compute shaders with shaders of other types (vertex, fragment, ect..)
*/

class CompShaderLoader : public ShaderLoader
{
public:
	// the program ID
	unsigned int ID;

	CompShaderLoader(unsigned int ID = 0) : ID(ID) {}
	CompShaderLoader(const char* computePath);
	~CompShaderLoader();

	void useShaderStorageBuffer(GLsizeiptr size, void* data);

	// use/activate the shader
	void use() { glUseProgram(ID); };

private:
	GLuint BuffObjNameArry = 0;
};
#endif
