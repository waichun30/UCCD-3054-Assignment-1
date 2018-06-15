/* main.cpp
   Assignment_1_SupportCodes

   Created by Seow Hui, SAW on 11/06/2018.
   Copyright © 2018 SSH. All rights reserved.

   This support codes are obtained from MITOPENCOURSEWARE
   https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-837-computer-graphics-fall-2012/assignments/
*/

#include "main.hpp"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
/* Just in case we need these later
 // References:
 // http://alumni.cs.ucsb.edu/~wombatty/tutorials/opengl_mac_osx.html
 // # include <OpenGL/gl.h>
 // # include <OpenGL/glu.h>
 */
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "vecmath.h"
using namespace std;

// Globals

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes
GLuint colorCounter = 0;
GLfloat lightXPos = 0.0f;
GLfloat lightYPos = 0.0f;
GLfloat angle = 0.0f;
GLboolean rotateX = GL_FALSE;
GLfloat oldX = 0;
GLfloat oldY = 0;
GLfloat newX = 0;
GLfloat newY = 0;
GLfloat moveX = 0;
GLfloat moveY = 0;
GLfloat zoom = 0;
GLboolean mouseClick = GL_FALSE;
GLboolean scroll = GL_FALSE;
GLfloat angleY = 0;
GLfloat angleX = 0;
GLboolean random = GL_FALSE;
GLboolean resize = GL_FALSE;
GLboolean hor = GL_FALSE;
GLboolean ver = GL_FALSE;
GLfloat horizontalCam = 0.0f;
GLfloat verticalCam = 0.0f;
GLboolean condition1 = GL_FALSE;
GLboolean condition2 = GL_FALSE;


// These are convenience functions which allow us to call OpenGL
// methods on Vec3d objects
inline void glVertex(const Vector3f &a)
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a)
{ glNormal3fv(a); }


// This function for rotate/spin the teapot
void timer(int value)
{
	//case 'r'
	if (rotateX == GL_TRUE) {
		moveX = 0;
		moveY = 0;
		angle += 1;
		glutTimerFunc(10, timer, 0);
	}

	//special case 'a'
	if (random == GL_TRUE) {
		moveX = 0;
		moveY = 0;
		angle += 1;
		glutTimerFunc(10, timer, 0);
	}
	glutPostRedisplay();
}



// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y)
{
    switch ( key )
    {
        case 27: // Escape key
            exit(0);
            break;
        case 'c':
            // add code to change color here
			colorCounter += 1;
            cout << "Unhandled key press " << key << "." << endl;
            break;

		case 'r':// rotate
			if (condition1 == GL_FALSE) {
				if (rotateX == GL_FALSE)
				{
					condition2 = GL_TRUE;
					rotateX = GL_TRUE;

					cout << "Unhandled key press " << key << "." << endl;
				}
				else {
					rotateX = GL_FALSE;
					condition2 = GL_FALSE;

				}
				mouseClick = GL_FALSE;
				scroll = GL_FALSE;
				glutTimerFunc(10, timer, 0);
			}
				
			break;

		case 'a': //special feature
			if (condition2 == GL_FALSE) {
				if (random == GL_FALSE) {

					cout << "Unhandled key press " << key << "." << endl;
					random = GL_TRUE;
					condition1 = GL_TRUE;
					glutTimerFunc(10, timer, 0);

				}
				else {
					random = GL_FALSE;
					condition1 = GL_FALSE;

				}

			}
			break;
        default:
            cout << "Unhandled key press " << key << "." << endl;
    }

    // this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}



// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
        case GLUT_KEY_UP:
            // add code to change light position
			lightYPos += 0.5f;
            cout << "Unhandled key press: up arrow." << endl;
            break;
        case GLUT_KEY_DOWN:
            // add code to change light position
			lightYPos += -0.5f;
            cout << "Unhandled key press: down arrow." << endl;
            break;
        case GLUT_KEY_LEFT:
            // add code to change light position
			lightXPos += -0.5f;
            cout << "Unhandled key press: left arrow." << endl;
            break;
        case GLUT_KEY_RIGHT:
            // add code to change light position
			lightXPos += 0.5f;
            cout << "Unhandled key press: right arrow." << endl;
            break;
    }
    
    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}


// Mouse Event
void OnMouseClick(int button, int state, int x, int y) {
	
	//scroll up
	if (button == 3) {
		scroll = GL_TRUE;
		zoom += -0.5;
		cout << "Unhandled key press: scroll up." << endl;
	}
	//scroll down
	if (button == 4) {
		scroll = GL_TRUE;
		zoom += 0.5;
		cout << "Unhandled key press: scroll down." << endl;
	}

	// left click pressed 
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		oldX = x;
		oldY = y;
		cout << "Unhandled key press: left click pressed." << endl;
	}

	//left click released
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		newX = x;
		newY = y;
		moveX = (newX - oldX);
		moveY = (newY - oldY);
		oldX = x;
		oldY = y;
		mouseClick = GL_TRUE;

		cout << "Unhandled key press: left click released." << endl;
	}

	
	glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;
    
    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Rotate the image
    glMatrixMode( GL_MODELVIEW);  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity
    

	//
    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0+zoom,
              0.0+horizontalCam, 0.0+verticalCam, 0.0 ,
              0.0, 1.0, 0.0 );
    
    // Set material properties of object
    
    // Here are some colors you might use - feel free to add more
	// Added 4 color
    GLfloat diffColors[8][4] = { {0.5, 0.5, 0.9, 1.0},
        {0.9, 0.5, 0.5, 1.0},
        {0.5, 0.9, 0.3, 1.0},
		{ 0.5, 0.9, 0.3, 7.0 },
		{ 0.5, 0.9, 1.2, 1.0 },
		{ 0.5, 1.9, 0.3, 1.0 },
		{ 1.5, 0.9, 0.3, 1.0 },
        {0.3, 0.8, 0.9, 1.0} };
    
    // Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[colorCounter%7]);
    
    // Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};
    
    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    
    // Set light properties
    
    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
    GLfloat Lt0pos[] = {1.0f+lightXPos, 1.0f+lightYPos, 5.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
	
	// case block

	// question2.2 animation(spin) 
	if (rotateX) {
		angleX = angle;
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
	}

	// question 2.3 interaction with mouse (button click only)
	else if(mouseClick) {
		glRotatef(angleY + moveY, 1.0f, 0.0f, 0.0f);
		glRotatef(angleX + moveX, 0.0f, 1.0f, 0.0f);	
		mouseClick = GL_FALSE;
		scroll = GL_FALSE;
		angleY += moveY;
		angleX += moveX;
		angle = angleX;
	}

	// extra features , press 'a'
	else if (random) {

		// zoom in and zoom out
		if (resize) {
			zoom -= 0.02;
			if (zoom < 0) {
				resize = GL_FALSE;
			}
		}
		else {
			zoom += 0.02;
			if (zoom > 10) {
				resize = GL_TRUE;
			}
		}

		// horizontal camera view
		if (hor) {
			horizontalCam += 0.01;
			if (horizontalCam > 1.0)
				hor = GL_FALSE;
		}
		else {
			horizontalCam -= 0.01;
			if (horizontalCam < -1.0)
				hor = GL_TRUE;
		}

		// vertical camera view
		if (ver) {
			verticalCam += 0.01;
			if (verticalCam > 1.0)
				ver = GL_FALSE;
		}
		else {
			verticalCam -= 0.01;
			if (verticalCam < -1.0)
				ver = GL_TRUE;
		}
			
		
		// rotate/spin
		angleX += 1;
		angleY += 1;
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);
		glRotatef(angleX, 0.0f, 1.0f, 0.0f);
	}

	// question 2.3 interaction with mouse (scroll )
	else if (scroll)
	{
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);
		glRotatef(angleX, 0.0f, 1.0f, 0.0f);
	}
	else {
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
	}

    // This GLUT method draws a teapot.  You should replace
    // it with code which draws the object you loaded.
    glutSolidTeapot(1.0);

    
    // Dump the image to the screen.
    glutSwapBuffers();
}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }
    
    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput()
{
    // load the OBJ file here
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();
    
    glutInit(&argc,argv);
    
    // We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment ONE");
    
    // Initialize OpenGL parameters.
    initRendering();
    
    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys
	glutMouseFunc(OnMouseClick);
    
    // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );
    
    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );
    
    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );
    
    return 0;    // This line is never reached.
}
