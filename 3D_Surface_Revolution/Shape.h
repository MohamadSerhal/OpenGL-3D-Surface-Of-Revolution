#pragma once

#include "glsupport.h"
#include "matrix4.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "BSpline.h"
#include "cvec.h"
#include <iostream>


class Shape
{
	GLuint vertsVBO, facesIBO;

	GLfloat verts[100000][3];
	GLuint faces[100000];
	
	int number_of_verts = 0;
	int number_of_faces = 0;

	float y_roll, x_roll;

public:
	void rotate(float theta_x, float theta_y) {
		y_roll += theta_y;
		x_roll += theta_x;
		

	}
	Shape();
	void calculateVertexPositions(BSpline curve);
	void constructFaces(BSpline curve);
	void drawObj(GLuint h_aVertex, GLuint h_transformations);
	void initVBOs();
};

