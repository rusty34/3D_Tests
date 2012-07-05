/** Copyright (C) 2012 by Shane R. Porter **/
/** This file is licensed under the MIT License **/

#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <stdexcept>

#include <glload/gl_3_3_comp.h>
#include <glload/gll.hpp>
#include <glutil/Shader.h>
#include <GL/freeglut.h>

#include "main.h"

GLuint program;
GLuint offsetLocation;
GLuint perspectiveMatrixLocation;

float perspectiveMatrix[16];
const float fFrustumScale = 1.0f;

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

int main(int argc, char** argv)
{

	glutInit(&argc, argv);

	int width = 500;
	int height = 500;

	unsigned int displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

	glutInitDisplayMode(displayMode);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	#ifndef DEBUG
	glutInitContextFlags(GLUT_DEBUG);
	#endif

	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 200);
	int window = glutCreateWindow("Test");

	glload::LoadFunctions();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

//Initialise shaders and data used in the shaders
void InitialiseProgram()
{
	std::vector<GLuint> shaderList;
	shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "data\\VertexShader.vert"));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "data\\FragmentShader.frag"));

	program = Framework::CreateProgram(shaderList);

	offsetLocation = glGetUniformLocation(program, "offset");

	perspectiveMatrixLocation = glGetUniformLocation(program, "perspectiveMatrix");

	float fzNear = 0.5f;
	float fzFar = 3.0f;

	memset(perspectiveMatrix, 0, sizeof(float) * 16);

	perspectiveMatrix[0] = fFrustumScale;
	perspectiveMatrix[5] = fFrustumScale;
	perspectiveMatrix[10] = (fzFar + fzNear) / (fzNear - fzFar);
	perspectiveMatrix[14] = (2 * fzFar * fzNear) / (fzNear - fzFar);
	perspectiveMatrix[11] = -1.0f;

	glUseProgram(program);
	glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_FALSE, perspectiveMatrix);
	glUseProgram(0);
}

//Buffer that stores a 3D rectangular box
const float vertexData[] = {
	 0.25f,  0.25f, -1.25f, 1.0f,
	 0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,

	 0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,

	 0.25f,  0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,
	 0.25f, -0.25f, -2.75f, 1.0f,

	 0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,

	-0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,

	-0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,

	 0.25f,  0.25f, -1.25f, 1.0f,
	 0.25f, -0.25f, -2.75f, 1.0f,
	 0.25f, -0.25f, -1.25f, 1.0f,

	 0.25f,  0.25f, -1.25f, 1.0f,
	 0.25f,  0.25f, -2.75f, 1.0f,
	 0.25f, -0.25f, -2.75f, 1.0f,

	 0.25f,  0.25f, -2.75f, 1.0f,
	 0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,

	 0.25f,  0.25f, -2.75f, 1.0f,
	-0.25f,  0.25f, -1.25f, 1.0f,
	-0.25f,  0.25f, -2.75f, 1.0f,

	 0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,
	 0.25f, -0.25f, -1.25f, 1.0f,

	 0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -2.75f, 1.0f,
	-0.25f, -0.25f, -1.25f, 1.0f,




	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,

	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

};

GLuint vertexBufferObject;
GLuint vao;

//Initialise buffer objects
void InitialiseVertexBuffer()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Initialise the shaders + buffer objects
void init()
{
	InitialiseProgram();
	InitialiseVertexBuffer();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}

//Computes a circular offset to move something around
void ComputeOffset(float &fXOffset, float &fYOffset)
{
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.5f;
	fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.5f;
}

//Main drawing method
void display()
{
	//Offset Testing
	float fXOffset = 0.0f;
	float fYOffset = 0.0f;
	ComputeOffset(fXOffset, fYOffset);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glUniform2f(offsetLocation, fXOffset, fYOffset);

	size_t colorData = sizeof(vertexData) / 2;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

//Called when the window is resized
//Updates perspectiveMatrix to make everything have the correct perspective.
void reshape(int w, int h)
{
	perspectiveMatrix[0] = fFrustumScale / (w / (float)h);
	perspectiveMatrix[5] = fFrustumScale;

	glUseProgram(program);
	glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_FALSE, perspectiveMatrix);
	glUseProgram(0);

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

//Called when a key is pressed
//key variable is the pressed key in ASCII
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
		glutLeaveMainLoop();
		return;
	}
}