/** Copyright (C) 2012 by Shane R. Porter **/
/** This file is licensed under the MIT License. **/

#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <string>

namespace Framework
{
	float DegToRad(float fAngDeg);

	/*GLUint CreateShader(GLenum shaderType, 
	                    const std::string &shaderFile, 
						const std::string &shaderName);*/
	GLuint LoadShader(GLenum shaderType, const std::string &shaderFile);

	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
}

void InitialiseProgram();
void InitialiseVertexBuffer();

void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);


#endif