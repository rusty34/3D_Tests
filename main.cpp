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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "main.h"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

GLuint program;

GLuint modelToCameraMatrixUnif;
GLuint cameraToClipMatrixUnif;

glm::mat4 cameraToClipMatrix(1.0f);


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


float CalcFrustumScale(float fFovDeg)
{
	float fFovRad = Framework::DegToRad(fFovDeg);
	return 1.0f / tan(fFovRad / 2.0f);
}

const float fFrustumScale = CalcFrustumScale(45.0f);

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


	modelToCameraMatrixUnif = glGetUniformLocation(program, "modelToCameraMatrix");
	cameraToClipMatrixUnif = glGetUniformLocation(program, "cameraToClipMatrix");

	float fzNear = 1.0f;
	float fzFar = 45.0f;

	cameraToClipMatrix[0].x = fFrustumScale;
	cameraToClipMatrix[1].y = fFrustumScale;
	cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
	cameraToClipMatrix[2].w = -1.0f;
	cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

	glUseProgram(program);
	glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
	glUseProgram(0);
}

//Buffer that stores a 3D rectangular box
const int numberOfVertices = 8; 

#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f

const float vertexData[] = {
	//Object 1 positions
	-1.0f, +1.0f, +1.0f,
	+1.0f, +1.0f, +1.0f,
	+1.0f, -1.0f, +1.0f,
	-1.0f, -1.0f, +1.0f,

	-1.0f, +1.0f, -1.0f,
	+1.0f, +1.0f, -1.0f,
	+1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,

	//Object 1 color
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,
	BLUE_COLOR,

	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	RED_COLOR,
	
};

const GLshort indexData[] =
{
	//Front
	0, 1, 2,
	2, 3, 0,

	//Top
	0, 4, 1,
	1, 4, 5,

	//Bottom
	3, 2, 7,
	2, 6, 7,

	//Left
	0, 3, 7,
	7, 4, 0,

	//Right
	2, 1, 6,
	5, 6, 1,

	//Back
	4, 5, 6,
	6, 7, 4,

};

GLuint vertexBufferObject;
GLuint indexBufferObject;
GLuint vao;

//Initialise buffer objects
void InitialiseVertexBuffer()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void InitialiseVertexArrayObject()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

	glBindVertexArray(0);
}

//Initialise the shaders + buffer objects
void init()
{
	InitialiseProgram();
	InitialiseVertexBuffer();
	InitialiseVertexArrayObject();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	//glEnable(GL_DEPTH_CLAMP);
}

//Computes a circular offset to move something around
glm::mat4 ConstructMatrix()
{
	glm::mat4 transMat(1.0f);

	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	glm::vec3 transVec = glm::vec3(cosf(fCurrTimeThroughLoop * fScale) * 4.0f,
	          sinf(fCurrTimeThroughLoop * fScale) * 4.0f,
	          -10.0f);

	transMat[3] = glm::vec4(transVec, 1.0f);

	return transMat;
}

//Main drawing method
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glBindVertexArray(vao);

	const glm::mat4 &transformMatrix = ConstructMatrix();

	glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

//Called when the window is resized
//Updates perspectiveMatrix to make everything have the correct perspective.
void reshape(int w, int h)
{
	cameraToClipMatrix[0].x = fFrustumScale / (w / (float)h);
	cameraToClipMatrix[1].y = fFrustumScale;

	glUseProgram(program);
	glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
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