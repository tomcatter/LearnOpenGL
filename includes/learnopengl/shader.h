#ifndef SHADER_H
#define SHADER_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID;
	
	// constructor generates the shader on the fly
	// -------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath, const char* geomertyPath = nullptr)
	{
		
	}
};

#endif