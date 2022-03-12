#include "glsupport.h"
#include "BSpline.h"
#include "Shape.h"

static int g_width = 700;
static int g_height = 700;

static GLuint h_program2D, h_program3D;
static GLuint h_aVertex2D, h_aVertex3D, h_transformations;

static Shape surface_of_revolution;

static BSpline curve;

static int mouse_position[2] = {500, 500};

static bool allow_display = false;

enum Mode { EDIT, DISPLAY };
Mode mode = EDIT;

void display() {

	if (mode == Mode::EDIT) {
		glUseProgram(h_program2D);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		curve.drawObj(h_aVertex2D);
		glutSwapBuffers();
		// check for errors
		if (glGetError() != GL_NO_ERROR) {
			//const GLubyte* errString;
			//errString = gluErrorString(errCode);
			//printf("error: %s\n", errString);
			printf("error: \n");
		}
	}
	
	else if (mode == Mode::DISPLAY) {
		glUseProgram(h_program3D);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		surface_of_revolution.drawObj(h_aVertex3D, h_transformations);
		glutSwapBuffers();
		// check for errors
		if (glGetError() != GL_NO_ERROR) {
			//const GLubyte* errString;
			//errString = gluErrorString(errCode);
			//printf("error: %s\n", errString);
			printf("error: \n");
		}
	}
}


void reshape(int w, int h) {
	g_width = w;
	g_height = h;
	glViewport(0, 0, w, h);
	glutPostRedisplay();
}



void curveInput(unsigned char key, int xmouse, int ymouse) {

	if (mode == Mode::EDIT) {
		switch (key) {
		case 'n':
			curve.resetCtrlPoints();
			curve.initVBOs();
			glutPostRedisplay();
			break;

		case 'f':
			curve.initCurve();
			curve.initVBOs();
			glutPostRedisplay();
			allow_display = true;
			break;

		case 'm':
			if (allow_display) {
				mode = Mode::DISPLAY;
				surface_of_revolution.calculateVertexPositions(curve);
				surface_of_revolution.constructFaces(curve);
				surface_of_revolution.initVBOs();
				glutPostRedisplay();
			}
			
		default:
			break;
		}
	}

	if (mode == Mode::DISPLAY) {
		switch (key){
		case 'n':   // press n to start new b spline
			curve.resetCtrlPoints();
			curve.initVBOs();
			mode = Mode::EDIT;
			allow_display = false;
			glutPostRedisplay();
		default:
			break;
		}
	}
}



void mousepress(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		switch (mode) {
		case Mode::EDIT: {
			float x = (float)mx / (g_width / 2.0) - 1.0;
			float y = -1 * (my / (g_height / 2.0) - 1.0);
			curve.addCtrlPoint(x, y);
			curve.initVBOs();
			break;
		}
		case Mode::DISPLAY:
		{
			mouse_position[0] = mx;
			mouse_position[1] = my;
		}
		}
		glutPostRedisplay();
	}
}

void mousemove(int mx, int my) {
	if (mode == Mode::DISPLAY) {
		float horizontal_rot = 1.0 * (mx - mouse_position[0]) / g_height;
		float vertical_rot = 1.0* (my - mouse_position[1]) / g_width;
		horizontal_rot *= 100 * M_PI;
		vertical_rot *= 75 * M_PI;
		surface_of_revolution.rotate(vertical_rot, horizontal_rot);
		mouse_position[0] = mx;
		mouse_position[1] = my;
		glutPostRedisplay();
	}
}

void initGlutState() {
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(g_width, g_height);
	glutCreateWindow("3D Surface Of Revolution");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(curveInput);
	glutMouseFunc(mousepress);
	glutMotionFunc(mousemove);
}

void initGLState()
{
	glClearColor(128. / 255, 200. / 255, 1, 0);
	glEnable(GL_FRAMEBUFFER_SRGB); 
	glClearDepth(0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}


void initShaders()
{
	h_program2D = glCreateProgram();
	readAndCompileShader(&h_program2D,
		"./shaders/simple.vshader",
		"./shaders/simple.fshader");
	h_aVertex2D = safe_glGetAttribLocation(h_program2D, "aVertex");
	h_program3D = glCreateProgram();
	readAndCompileShader(&h_program3D,
		"./shaders/vertex3D.vshader",
		"./shaders/simple.fshader");
	h_aVertex3D = safe_glGetAttribLocation(h_program3D, "aVertex");
	h_transformations = safe_glGetUniformLocation(h_program3D, "transformations");
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	initGlutState();
	glewInit();
	initGLState();
	initShaders();
	glutMainLoop();
	return 0;
}