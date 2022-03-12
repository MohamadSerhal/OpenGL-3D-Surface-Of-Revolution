#pragma once


#include<vector>
#include "glsupport.h"
class vect2 {
public:
	float x, y;
	vect2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};
class BSpline
{



	std::vector<vect2> ctrlPoints;
	GLfloat ctrlArray[80];
public:
	GLuint curveVertBO, ctrlPtsBO;

	int numOfVerts = 0;
	GLfloat* verts = NULL;
	void addCtrlPoint(float x, float y);
	void resetCtrlPoints();
	void initCurve();
	void initVBOs();
	void drawObj(GLuint h_aVertex);
};

