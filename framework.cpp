
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <string.h>

#include <glload/gl_3_3_comp.h>
#include <glutil/Shader.h>
#include <GL/freeglut.h>

#include "framework.h"

namespace Framework
{
	//Converts degress to radians
	float DegToRad(float fAngDeg)
	{
		const float fDegToRad = 3.14159f * 2.0f / 360.0f;
		return fAngDeg * fDegToRad;
	}

	//Reads in file and compiles/creates a shader from it
	GLuint LoadShader(GLenum shaderType, const std::string &shaderFilename)
	{
		std::ifstream shaderFile(shaderFilename.c_str());
		std::stringstream shaderData;
		shaderData << shaderFile.rdbuf();
		shaderFile.close();

		try
		{
			return glutil::CompileShader(shaderType, shaderData.str());
		}
		catch(std::exception &e)
		{
			std::cout << "Shader Load Error\n";
			fprintf(stderr, "%s\n", e.what());
			throw;
		}
	}

	//Creates the program and links shaders with it
	GLuint CreateProgram(const std::vector<GLuint> &shaderList)
	{
		try
		{
			GLuint prog = glutil::LinkProgram(shaderList);
			std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
			return prog;
		}
		catch(std::exception &e)
		{
			std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
			fprintf(stderr, "%s\n", e.what());
			throw;
		}
	}
}