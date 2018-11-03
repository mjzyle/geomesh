#define _CRT_SECURE_NO_WARNINGS

#include "Vector.cpp"
#include "Face3.cpp"
#include "Face4.cpp"
#include "Mesh3.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <GL\glut.h>
using namespace std;

#define ON 1
#define OFF 0

// Window variables
int window_width, window_height;
int PERSPECTIVE = ON;

// Camera variables
float lastX, lastY;
int moveCam = ON;
float radius = 5, theta = 0, psi = 5, sensitivity = 0.05;

// Display variables
int showAxes = ON, showObject = ON;

// Object variables
Mesh3 m1, m2;

// Shader variables
GLuint vertex_shader, fragment_shader;


void display(void) {
	// Clear the background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (PERSPECTIVE) {
		// Perpective Projection 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLdouble)window_width / window_height, 0.01, 10000);
		glutSetWindowTitle("Assignment 2 Template (perspective)");
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else {
		// Orthogonal Projection 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-2.5, 2.5, -2.5, 2.5, -10000, 10000);
		glutSetWindowTitle("Assignment 2 Template (orthogonal)");
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// Set the camera position (based on spherical coordinates), orientation and target
	// Due to modified coordinate system in OpenGL: x in the normal Cartesian system becomes y, y becomes z, and z becomes x
	gluLookAt(radius*sin(theta)*sin(psi), radius*cos(psi), radius*cos(theta)*sin(psi), 0, 0, 0, 0, 1, 0);

	// Draw the mesh
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (showObject) {
		for (int i = 0; i < m1.faces; i++) {
			glBegin(GL_TRIANGLES);
			glVertex3f(m1.face_list[i].a.x, m1.face_list[i].a.y, m1.face_list[i].a.z);
			glVertex3f(m1.face_list[i].b.x, m1.face_list[i].b.y, m1.face_list[i].b.z);
			glVertex3f(m1.face_list[i].c.x, m1.face_list[i].c.y, m1.face_list[i].c.z);
			glEnd();
		}
	}

	if (showAxes) {
		// Draw the coordinate axes
		// x-axis in green
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glEnd();
		// y-axis in red
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		glEnd();
		// z-axis in blue
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}

	// (Note that the origin is lower left corner)
	// (Note also that the window spans (0,1) )
	// Finish drawing, update the frame buffer, and swap buffers
	glutSwapBuffers();
}

void resize(int x, int y) {
	glViewport(0, 0, x, y);
	window_width = x;
	window_height = y;
	if (PERSPECTIVE) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLdouble)window_width / window_height, 0.01, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	printf("Resized to %d %d\n", x, y);
}

void mouseButton(int button, int state, int x, int y) {
	lastX = x;
	lastY = y;
	// Left click
	if (button == 0) {
		// Press
		if (state == 0) {
			// Tumble the camera
			moveCam = ON;
		}
	}
	// Right click
	if (button == 2) {
		// Press
		if (state == 0) {
			// Zoom the camera
			moveCam = OFF;
		}
	}
	printf("Mouse click at %d %d, button: %d, state %d\n", x, y, button, state);
}

void mouseMotion(int x, int y) {
	// Rotating the camera to the right/moving mouse to left
	if (lastX > x && moveCam) {
		theta = theta - sensitivity;
	}
	// Raising the camera upwards/moving mouse up
	if (lastY > y && moveCam) {
		psi = psi + sensitivity;
	}
	// Rotating the camera to the left/moving mouse to right
	if (lastX < x && moveCam) {
		theta = theta + sensitivity;
	}
	// Lowering the camera/moving mouse down
	if (lastY < y && moveCam) {
		psi = psi - sensitivity;
	}
	// Zooming in
	if (lastY > y && !moveCam) {
		radius = radius - sensitivity;
	}
	// Zooming out
	if (lastY < y && !moveCam) {
		radius = radius + sensitivity;
	}
	printf("Mouse is at %d, %d\n", x, y);
	// Initialize lastX and lastY for next mouse position
	lastX = x;
	lastY = y;
	printf("Mouse is at %d, %d\n", x, y);
	// Schedule next display event
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'q':                           /* Quit */
		case 'Q':
			exit(1);
			break;
		case 'p':
		case 'P':
			// Toggle Projection Type (orthogonal, perspective)
			if (PERSPECTIVE) {
				// switch from perspective to orthogonal
				PERSPECTIVE = OFF;
			}
			else {
				// switch from orthogonal to perspective
				PERSPECTIVE = ON;
			}
			break;
		case 'a':
		case 'A':
			// Toggle axes visibility
			if (showAxes) {
				// hide axes
				showAxes = OFF;
			}
			else {
				// show axes
				showAxes = ON;
			}
			break;
		case 's':
		case 'S':
			// Toggle object visibility
			if (showObject) {
				// hide object
				showObject = OFF;
			}
			else {
				// show object
				showObject = ON;
			}
			break;
		default:
			break;
	}

	// Schedule a new display event
	glutPostRedisplay();
}

void setShaders() {

}


// Main method
int main(int argc, char* argv[]) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Geodesic Shape");
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);

	// Initialize GL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.5, 2.5, -2.5, 2.5, -10000, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	// Load objects
	m1 = Mesh3("sphere.obj");
	m1.geodesic();

	// Setup shaders and textures
//	setShaders();

	// Start render loop
	glutMainLoop();


	return 0;
}