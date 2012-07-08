
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <vector>
#include <string>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace Framework
{
	//Converts degrees to radians
	float DegToRad(float fAngDeg);

	//Reads in Shader from file.
	GLuint LoadShader(GLenum shaderType, const std::string &shaderFile);

	//Creates program with shaders
	//Will delete shader when finished
	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
}

#endif