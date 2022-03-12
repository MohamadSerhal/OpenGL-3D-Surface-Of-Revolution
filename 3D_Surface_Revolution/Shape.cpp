#include "Shape.h"

Shape::Shape()
{
	x_roll = 0;
	y_roll = 0;
}


void Shape::calculateVertexPositions(BSpline curve) {
	int Ncircle = 25;  // number of points per circle
	int Ncurve = curve.numOfVerts;
	float delta = (float)(2 * M_PI / (Ncircle - 1));
	for (size_t i = 0; i < Ncurve; i++)
	{
		float theta = 0;
		float radius = curve.verts[2 * i];
		float y = curve.verts[2 * i + 1];
		for (size_t j = 0; j < Ncircle; j++)
		{
			int index = i * Ncircle + j;
			verts[index][0] = (GLfloat)(radius * cos(theta));
			verts[index][1] = y;
			verts[index][2] = (GLfloat)(radius * sin(theta));
			theta += delta;
		}
	}

	number_of_verts = Ncircle * Ncurve;

	
}



void Shape::constructFaces(BSpline curve) {
	int Ncircle = 25;  // number of points per circle
	int Ncurve = curve.numOfVerts;
	for (size_t i = 0; i < Ncurve - 1; i++)
	{
		for (size_t j = 0; j < Ncircle; j++)
		{
			GLuint A = i * Ncircle + j;
			GLuint index = A;
			GLuint B = i * Ncircle + (j + 1) % Ncircle;
			GLuint C = (i + 1) * Ncircle + j;
			GLuint D = (i + 1) * Ncircle + (j + 1) % Ncircle;
			faces[3 * index] = A;
			faces[3 * index + 1] = D;
			faces[3 * index + 2] = B;
			index++;
			faces[3 * index] = A;
			faces[3 * index + 1] = C;
			faces[3 * index + 2] = D;
			index++;
		}
	}
	number_of_faces = (Ncurve - 1) * Ncircle * 2;
	
}


void Shape::initVBOs()
{
	glGenBuffers(1, &vertsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertsVBO);
	glBufferData(GL_ARRAY_BUFFER, number_of_verts * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

	glGenBuffers(1, &facesIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_of_faces * 3 * sizeof(GLuint), faces, GL_STATIC_DRAW);

}


void Shape::drawObj(GLuint h_aVertex, GLuint h_transformations)
{
	Matrix4 rotation = Matrix4::makeYRotation(y_roll) * Matrix4::makeXRotation(x_roll);
	Matrix4 translation = Matrix4::makeTranslation(Cvec3(0, 0, -1.25));
	Matrix4 projection = Matrix4::makeProjection(90, 1, -0.1, -50);
	GLfloat mat[16];
	Matrix4 total = projection * translation * rotation;
	total.writeToColumnMajorMatrix(mat);
	glUniformMatrix4fv(h_transformations, 1, GL_FALSE, mat);
	glBindBuffer(GL_ARRAY_BUFFER, vertsVBO);
	safe_glVertexAttribPointer(h_aVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	safe_glEnableVertexAttribArray(h_aVertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesIBO);
	glDrawElements(GL_LINE_LOOP, number_of_faces * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_POINTS, 0, number_of_verts);
	safe_glDisableVertexAttribArray(h_aVertex);
}

