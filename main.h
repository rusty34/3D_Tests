/** Copyright (C) 2012 by Shane R. Porter **/
/** This file is licensed under the MIT License. **/

#ifndef MAIN_H
#define MAIN_H

void InitialiseProgram();
void InitialiseVertexBuffer();

void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

float boxYOffset = 0.0f;
float boxXOffset = 0.0f;
float increment = 0.1f;

#endif