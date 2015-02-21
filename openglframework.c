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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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

GLfloat eyePosition[3] = { 0.0f, 0.0f, 5.0f };
GLfloat coordinates[3] = { 0.0f, 0.0f, 0.0f };

bool wPressed=false, sPressed=false, aPressed=false, dPressed=false, iPressed=false, kPressed=false, tPressed=false, gPressed=false, fPressed=false, hPressed=false, rPressed=false, yPressed=false, spacePressed=false;


void display(void);
void computeMovement();
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void reshape(int w, int h);

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif


    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
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
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);
    glutReshapeFunc(reshape);


    glutMainLoop();

    return 0;
}

void display(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f,0.0f,1.0f);
    glLoadIdentity();
    computeMovement();
    gluLookAt(eyePosition[0],eyePosition[1],eyePosition[2],coordinates[0],coordinates[1],coordinates[2],0.0,1.0,0.0);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    
    glColorPointer(3,GL_FLOAT, 0, cubeColors);
    
    // draw a cube
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
    
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void computeMovement()
{
    if (wPressed)
    {
        eyePosition[2] -= 0.1f;
    }
    if (sPressed)
    {
        eyePosition[2] += 0.1f;
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
    if (tPressed)
    {
        coordinates[1] += 0.1f;
    }
    if (gPressed)
    {
        coordinates[1] -= 0.1f;
    }
    if (fPressed)
    {
        coordinates[0] -= 0.1f;
    }
    if (hPressed)
    {
        coordinates[0] += 0.1f;
    }
    if (rPressed)
    {
        coordinates[2] -= 0.1f;
    }
    if (yPressed)
    {
        coordinates[2] += 0.1f;
    }
    if (spacePressed)
    {
        
    }
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
            
    }

}

void reshape(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
}
