#include "BSpline.h"
static float r = 1.0 / 6;
void BSpline::addCtrlPoint(float x, float y) {
	
	ctrlArray[ctrlPoints.size() * 2] = x;
	ctrlArray[ctrlPoints.size() * 2 + 1] = y;
	ctrlPoints.push_back(vect2(x, y));
	if(ctrlPoints.size() == 1)
		addCtrlPoint(x, y);
	

}
void BSpline::resetCtrlPoints() {
	ctrlPoints.clear();
	numOfVerts = 0;
	verts = NULL;
}
void BSpline::initCurve() {
	
	if (verts != NULL)
	{
		delete(verts);
		numOfVerts = 0;
	}
	addCtrlPoint(ctrlPoints[ctrlPoints.size() - 1].x, ctrlPoints[ctrlPoints.size() - 1].y);
	int n = 8; // representing the number of points approximating each curve segment
	numOfVerts = (ctrlPoints.size() - 3) * n;//8 points between ctrl points
	verts = (GLfloat*)malloc((numOfVerts * 2) * sizeof(GLfloat));

	float dt = 1.0f/n;
	float t;
	int pt_index = 0;
	for (size_t i = 0; i < ctrlPoints.size()-3; i++)
	{
		t = 0;
		for (size_t j = 0; j < n; j++)
		{
			float t3 = t * t * t;
			float t2 = t * t;
			
			float B[4] = { r * (1 - 3 * t + 3 * t2 - t3),
							r * (3*t3 - 6 * t2 + 4),
							r * (-3*t3 + 3*t2 + 3 * t + 1),
							r * t3 };
			verts[pt_index * 2] = verts[pt_index * 2 + 1] = 0;
			for (size_t k = 0; k < 4; k++)
			{
				verts[pt_index * 2] += B[k] * ctrlPoints[i+k].x;
				verts[pt_index * 2 + 1] += B[k] * ctrlPoints[i+k].y;
			}
			t += dt;
			pt_index++;
		}

	}

}
void BSpline::initVBOs(void) {

	glGenBuffers(1, &curveVertBO);
	glBindBuffer(GL_ARRAY_BUFFER, curveVertBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		numOfVerts * 2 * sizeof(GLfloat),
		verts,
		GL_STATIC_DRAW);
	//ready the array to draw the control points separately
	if (ctrlPoints.size() > 0) {
		glGenBuffers(1, &ctrlPtsBO);
		glBindBuffer(GL_ARRAY_BUFFER, ctrlPtsBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			ctrlPoints.size() * 2 * sizeof(GLfloat),
			ctrlArray,
			GL_STATIC_DRAW);
	}

}

void BSpline::drawObj(GLuint h_aVertex) {
	glBindBuffer(GL_ARRAY_BUFFER, curveVertBO);
	safe_glVertexAttribPointer(h_aVertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	safe_glEnableVertexAttribArray(h_aVertex);
	glDrawArrays(GL_LINE_STRIP, 0, numOfVerts);
	glPointSize(3);
	glDrawArrays(GL_POINTS, 0, numOfVerts);
	safe_glDisableVertexAttribArray(h_aVertex);
	if (ctrlPoints.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, ctrlPtsBO);
		safe_glVertexAttribPointer(h_aVertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
		safe_glEnableVertexAttribArray(h_aVertex);
		glDrawArrays(GL_LINE_STRIP, 0, ctrlPoints.size());
		glPointSize(6);

		glDrawArrays(GL_POINTS, 0, ctrlPoints.size());
		safe_glDisableVertexAttribArray(h_aVertex);
	}
}