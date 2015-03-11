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
#define SPHERE_N (20)
#define FERMAT_RADIUS 10

#include "glm.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


GLuint windowDimensions[2] = { 400, 400 };

GLfloat objectsRotX = 0;
GLfloat objectsRotY = 0;

GLfloat rotY = 0; // Camera rotation aroud the y axis, in degrees
GLfloat rotX = 90; // Camera rotation around the x axis, in degrees

GLfloat eyePosition[3] = { 0.0, 0.0, 5.0 };
GLfloat lookAtPosition[3] = { 0.0, 0.0, 0.0 };

GLfloat old_x = 0, old_y = 0; // Stores the last mouse position
GLfloat horizontal_angle = 0, vertical_angle = 0; // How far is the mouse from the center of the window

bool wPressed=false, sPressed=false, aPressed=false, dPressed=false, upPressed=false, downPressed=false, leftPressed=false, rightPressed=false, commaPressed=false, dotPressed=false;

unsigned int apertureSamples = 16;

GLMmodel *modelPtr;
#define MODEL_FILENAME "obj/devilduk.obj"

GLuint bufferID;

void setLight();
void showStartMessage();
void display(void);
void updateCamera(unsigned int spiralIndex);
void computeMovement();
void onMouseDown(int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onSpecialInputDown(int key, int x, int y);
void onSpecialInputUp(int key, int x, int y);
void reshape(int w, int h);
void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n);
void loadModel();
int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
    GLenum err;
#endif

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM);
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

    initGLSLProgram("vertexshader.glsl","fragmentshader.glsl");
    
    /* Select clearing (background) color */
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
    glutDisplayFunc(display);
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);
    glutSpecialFunc(onSpecialInputDown);
    glutSpecialUpFunc(onSpecialInputUp);
    glutReshapeFunc(reshape);
    glutMotionFunc(onMouseDown);
    
    // Load OBJ model
    loadModel();
    glmInitVBO(modelPtr);

    //glutSetCursor(GLUT_CURSOR_NONE);
    
    showStartMessage();

    glutMainLoop();
    return 0;
}

void display(void)
{
    glClear(GL_ACCUM_BUFFER_BIT);
    
    for (int i = 0; i < apertureSamples; i++)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(0.0f,0.0f,1.0f);
        glLoadIdentity();
        
        computeMovement();
        setLight();
        updateCamera(i);
        
        glRotatef(objectsRotY,0,1,0);
        glRotatef(objectsRotX,1,0,0);
        
        // Insert code for glmDraw here
        //glmDraw(modelPtr, GLM_SMOOTH);
        glmInitVBO(modelPtr);
        glmDrawVBO(modelPtr, GLM_SMOOTH);
        
        glAccum(GL_ACCUM, 1.0/(double)apertureSamples);
    }
    
    glAccum(GL_RETURN, 1.0);
    glFlush();
    glutPostRedisplay();
    glutSwapBuffers();
}

void setLight()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat mat_ambient[] = { 1.0,1.0,1.0,1.0 };
    GLfloat mat_diffuse[] = { 1.0,1.0,1.0,1.0 };
    GLfloat mat_specular[] = {1.0,1.0,1.0,1.0};
    GLfloat light_position[] = {-200.0,600.0,1500.0, 1.0f};
    glClearColor (0.0,0.0,0.0,0.0);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
}

void updateCamera(unsigned int spiralIndex)
{
    double n = ((double)spiralIndex / (apertureSamples - 1)) * 0.727230441568491;
    double theta = n * 137.508;
    double radius = 0.001 * sqrt(theta);
    
    double eyeDisplacementX = radius * cos(theta*PI/180);
    double eyeDisplacementY = radius * sin(theta*PI/180);
    
    //printf("Theta: %f \t Displacement X: %f \t Displacement Y: %f\n", theta, eyeDisplacementX, eyeDisplacementY);
    //printf("EyePosition: %f, %f, %f", eyePosition[0] + eyeDisplacementX, eyePosition[1] + eyeDisplacementY, eyePosition[2]);
    //getchar();
    
    lookAtPosition[0] = eyePosition[0] + sin(rotY*PI/180);
    lookAtPosition[1] = eyePosition[1] + cos(rotX*PI/180);
    lookAtPosition[2] = eyePosition[2] - cos(rotY*PI/180);
    
    fflush(stdin);
    
    gluLookAt(eyePosition[0] + eyeDisplacementX, eyePosition[1] + eyeDisplacementY,eyePosition[2],lookAtPosition[0],lookAtPosition[1],lookAtPosition[2],0.0f, 1.0f, 0.0f);
}

void computeMovement()
{
    if (wPressed)
    {
        eyePosition[0] -= 0.005 * sin(rotY*PI/180);
        eyePosition[2] += 0.005 * -cos(rotY*PI/180);
    }
    if (sPressed)
    {
        eyePosition[0] += 0.005 * sin(rotY*PI/180);
        eyePosition[2] -= 0.005 * -cos(rotY*PI/180);
    }
    if (aPressed)
    {
        eyePosition[0] -= 0.005 * cos(rotY*PI/180);
        eyePosition[2] -= 0.005 * sin(rotY*PI/180);
    }
    if (dPressed)
    {
        eyePosition[0] += 0.005 * cos(rotY*PI/180);
        eyePosition[2] += 0.005 * sin(rotY*PI/180);
    }
    if (upPressed)
    {
        rotX += 0.25f;
    }
    if (downPressed)
    {
        rotX -= 0.25f;
    }
    if (leftPressed)
    {
        rotY -= 0.25f;
    }
    if (rightPressed)
    {
        rotY += 0.25f;
    }
}

void onMouseDown (int x, int y)
{
    if (x == 0 && y == 0)
    {
        old_x = x;
        old_y = y;
    }
    else
    {
        double delta_x = x - old_x;
        double delta_y = y - old_y;
        
        if (delta_x > 0) // Went right
        {
            objectsRotY += 2;
        }
        if (delta_x < 0) // Went left
        {
            objectsRotY -= 2;
        }
        if (delta_y > 0) // Went up
        {
            objectsRotX += 2;
        }
        if (delta_y < 0) // Went down
        {
            objectsRotX -= 2;
        }
        old_x = x;
        old_y = y;
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
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,0.1,20.0);
    glMatrixMode(GL_MODELVIEW);
    
    windowDimensions[0] = w;
    windowDimensions[1] = h;
}

void showStartMessage()
{
    puts("Computer Graphics Assignment - OpenGL:");
}

void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n)
{
    GLfloat ambient[] = {ka*r,ka*g,ka*b,1.0};
    GLfloat diffuse[] = {kd*r,kd*g,kd*b,1.0};
    GLfloat specular[] = {ks,ks,ks,1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, n);
}

void loadModel() {
    if (!modelPtr) {
        modelPtr = glmReadOBJ(MODEL_FILENAME);
        if (!modelPtr) {
            puts ("Error when loading model!!");
            exit(0);
        }
        
        glmUnitize(modelPtr);
        glmFacetNormals(modelPtr);
        glmVertexNormals(modelPtr, 90.0);
    }
}
