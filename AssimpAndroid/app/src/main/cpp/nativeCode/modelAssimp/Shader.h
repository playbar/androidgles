#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GLES2/gl2.h>


class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Init(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Uses the current shader
	void Use();
};

#endif