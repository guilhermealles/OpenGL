/*
* An OpenGL template/framework file for the Computer Graphics course
* at the University of Groningen.
*/

// If windows is used, windows.h should be included (before gl.h and glu.h)
#if defined(_WIN32)
#include <windows.h>
#endif

// If you use GLUT you do not need to include gl.h and glu.h
// as glut.h guarantees that gl.h and glu.h are properly 
// included.
// So if you do not wish to use  glut.h, uncomment the following lines.
//#include <GL/gl.h>
//#include <GL/glu.h>

#if defined(NEED_GLEW)
#include "glew.h"
#endif

// Apparently Mac OS X puts the GLUT headers somewhere different.
// For windows we use freeglut.
#if defined(__APPLE__)&& defined(__MACH__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265359

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

GLuint windowDimensions[2] = { 800, 600 };

GLfloat cubeVertices[8*3] =
{
    -1, 1,-1,
     1, 1,-1,
     1,-1,-1,
    -1,-1,-1,      // The first 4 vertices are the rear face of the cube
    
    -1, 1, 1,
     1, 1, 1,
     1,-1, 1,
    -1,-1, 1        // The last 4 vertices are the front face of the cube
};

GLubyte cubeIndices[4*6] =
{
    0,1,2,3,       // This is the rear face
    3,7,4,0,       // This is the left face
    4,5,6,7,       // This is the front face
    6,5,1,2,       // This is the right face
    0,1,5,4,       // This is the upper face
    7,3,2,6        // This is the lower face
};

GLfloat cubeColors[] =
{
    0.0,0.0,1.0,
    0.0,1.0,0.0,
    1.0,0.0,0.0,
    1.0,1.0,0.0,
    1.0,0.0,1.0,
    1.0,1.0,0.0,
    0.0,1.0,0.0,
    1.0,0.5,0.0,
};

GLfloat fieldOfViewY = 60.0f;

GLfloat rotY = 0; // Camera rotation in the y axis, in degrees
GLfloat rotX = 90; // Camera rotation in the x axis, in degrees

GLfloat eyePosition[3] = { 0.0f, 0.0f, 5.0f };
GLfloat lookAtPosition[3] = { 0.0f, 0.0f, 0.0f };
GLfloat cameraUpVector[3] = { 0.0f, 1.0f, 0.0f };


bool wPressed=false, sPressed=false, aPressed=false, dPressed=false, iPressed=false, kPressed=false, tPressed=false, gPressed=false, fPressed=false, hPressed=false, rPressed=false, yPressed=false, upPressed=false, downPressed=false, leftPressed=false, rightPressed=false, spacePressed=false, commaPressed=false, dotPressed=false;

void showStartMessage();
void display(void);
void updateCamera();
void computeKeyboardMovement();
void onMouseButton(int button, int state, int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onSpecialInputDown(int key, int x, int y);
void onSpecialInputUp(int key, int x, int y);
void reshape(int w, int h);

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif


    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowDimensions[0],windowDimensions[1]);
    glutInitWindowPosition(220,100);
    glutCreateWindow("Computer Graphics - OpenGL framework");

#if defined(NEED_GLEW)
    /* Init GLEW if needed */
    err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(1);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    /* Select clearing (background) color */
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
    glutDisplayFunc(display);
    glutMouseFunc(onMouseButton);
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);
    glutSpecialFunc(onSpecialInputDown);
    glutSpecialUpFunc(onSpecialInputUp);
    glutReshapeFunc(reshape);

    showStartMessage();
    
    glutMainLoop();
    return 0;
}

void display(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f,0.0f,1.0f);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfViewY,(GLdouble)windowDimensions[0]/(GLdouble)windowDimensions[1],1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    
    
    computeKeyboardMovement();
    updateCamera();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
        glColorPointer(3,GL_FLOAT, 0, cubeColors);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
    // Disable client states after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void updateCamera()
{
    lookAtPosition[0] = eyePosition[0] + sin(rotY*PI/180);
    lookAtPosition[1] = eyePosition[1] + cos(rotX*PI/180);
    lookAtPosition[2] = eyePosition[2] - cos(rotY*PI/180);
    
    gluLookAt(eyePosition[0],eyePosition[1],eyePosition[2],lookAtPosition[0],lookAtPosition[1],lookAtPosition[2],cameraUpVector[0],cameraUpVector[1],cameraUpVector[2]);
}

void computeKeyboardMovement()
{
    if (wPressed)
    {
        eyePosition[0] += 0.05 * sin(rotY*PI/180);
        eyePosition[2] += -0.05 * cos(rotY*PI/180);
    }
    if (sPressed)
    {
        eyePosition[0] -= 0.025 * sin(rotY*PI/180);
        eyePosition[2] -= -0.025 * cos(rotY*PI/180);
    }
    if (aPressed)
    {
        eyePosition[0] -= 0.1f; 
    }
    if (dPressed)
    {
        eyePosition[0] += 0.1f;
    }
    if (iPressed)
    {
        eyePosition[1] += 0.1f;
    }
    if (kPressed)
    {
        eyePosition[1] -=0.1f;
    }
    if (upPressed)
    {
        rotX += 4.0f;
    }
    if (downPressed)
    {
        rotX -= 4.0f;
    }
    if (leftPressed)
    {
        rotY -= 4.0f;
    }
    if (rightPressed)
    {
        rotY += 4.0f;
    }
    if (dotPressed)
    {
        if (fieldOfViewY < 180.0f) fieldOfViewY += 1;
    }
    if (commaPressed)
    {
        if (fieldOfViewY > 01.0f) fieldOfViewY -= 1;
    }
    if (spacePressed)
    {
        
    }
}

void onMouseButton(int button, int state, int x, int y)
{
    
}

void onKeyDown(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
        case 'W':
            wPressed = true;
            break;
        case 's':
        case 'S':
            sPressed = true;
            break;
        case 'a':
        case 'A':
            aPressed = true;
            break;
        case 'd':
        case 'D':
            dPressed = true;
            break;
        case ' ':
            spacePressed = true;
            break;
        case 'i':
        case 'I':
            iPressed = true;
            break;
        case 'k':
        case 'K':
            kPressed = true;
            break;
        case 't':
        case 'T':
            tPressed = true;
            break;
        case 'g':
        case 'G':
            gPressed = true;
            break;
        case 'r':
        case 'R':
            rPressed = true;
            break;
        case 'y':
        case 'Y':
            yPressed = true;
            break;
        case 'h':
        case 'H':
            hPressed = true;
            break;
        case 'f':
        case 'F':
            fPressed = true;
            break;
        case ',':
            commaPressed = true;
            break;
        case '.':
            dotPressed = true;
            break;
        case 'q':
        case 'Q':
        case 27: // ESC key
            printf("Exiting...\n");
            exit(0);
            break;

    }
}

void onKeyUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
        case 'W':
            wPressed = false;
            break;
        case 's':
        case 'S':
            sPressed = false;
            break;
        case 'a':
        case 'A':
            aPressed = false;
            break;
        case 'd':
        case 'D':
            dPressed = false;
            break;
        case ' ':
            spacePressed = false;
            break;
        case 'i':
        case 'I':
            iPressed = false;
            break;
        case 'k':
        case 'K':
            kPressed = false;
            break;
        case 't':
        case 'T':
            tPressed = false;
            break;
        case 'g':
        case 'G':
            gPressed = false;
            break;
        case 'r':
        case 'R':
            rPressed = false;
            break;
        case 'y':
        case 'Y':
            yPressed = false;
            break;
        case 'h':
        case 'H':
            hPressed = false;
            break;
        case 'f':
        case 'F':
            fPressed = false;
            break;
        case ',':
            commaPressed = false;
            break;
        case '.':
            dotPressed = false;
            break;
    }

}

void onSpecialInputDown(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            upPressed = true;
            break;
        case GLUT_KEY_DOWN:
            downPressed = true;
            break;
        case GLUT_KEY_LEFT:
            leftPressed = true;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = true;
            break;
    }
}

void onSpecialInputUp(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            upPressed = false;
            break;
        case GLUT_KEY_DOWN:
            downPressed = false;
            break;
        case GLUT_KEY_LEFT:
            leftPressed = false;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = false;
            break;
    }
}

void reshape(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfViewY,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    
    windowDimensions[0] = w;
    windowDimensions[1] = h;
}

void showStartMessage()
{
    puts("Computer Graphics Assignment - OpenGL:");
    puts("W, S, A, D - Move through X and Z Axis;");
    puts("Arrow Keys - Rotate the camera around the X and Y Axis;");
    puts(", and . - Change the fovy of the perspective;");
}
