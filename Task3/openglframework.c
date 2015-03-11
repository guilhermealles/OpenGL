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
#include <math.h>

#define PI 3.14159265359
#define SPHERE_N (20)

#define FERMAT_RADIUS 10                // Maximum radius of the sphere. The bigger, the more distorted the image is.
#define ATTENUATION_CONSTANT 0.001      // The bigger the value, the more distorted the image is. The displacement of the camera is very high even
                                        // when this constant is very low, so minimal change in this value will cause very big impact on the final image.
GLfloat maximumN;


GLuint windowDimensions[2] = { 400, 400 };

GLfloat spheresRotY = 0;
GLfloat spheresRotX = 0;

GLfloat rotY = 0; // Camera rotation aroud the y axis, in degrees
GLfloat rotX = 90; // Camera rotation around the x axis, in degrees

GLfloat eyePosition[3] = { 200.0, 200.0, 1000.0 };
GLfloat lookAtPosition[3] = { 200.0, 200.0, 0.0 };

GLfloat old_x = 0, old_y = 0; // Stores the last mouse position
GLfloat horizontal_angle = 0, vertical_angle = 0; // How far is the mouse from the center of the window
GLfloat direction_vector[3] = {0.0f, 0.0f, 0.0f};
GLfloat right_vector[3] = {0.0f, 0.0f, 0.0f};
GLfloat up_vector[3] = {0.0f, 0.0f, 0.0f};

bool wPressed=false, sPressed=false, aPressed=false, dPressed=false, upPressed=false, downPressed=false, leftPressed=false, rightPressed=false, spacePressed=false, commaPressed=false, dotPressed=false;

unsigned int apertureSamples = 16;

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
    
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    showStartMessage();

    //Calculate maximum N for the spiral displacement (Depth of Field)
    maximumN = pow(FERMAT_RADIUS, 2)/137.508f;
    
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
        
        glRotatef(spheresRotY,0,1,0);
        glRotatef(spheresRotX,1,0,0);
        
        setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.5,64);
        glPushMatrix();
        glTranslated(90,320,100);
        glutSolidSphere(50,SPHERE_N,SPHERE_N);
        glPopMatrix();
        
        setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);
        glPushMatrix();
        glTranslated(210,270,300);
        glutSolidSphere(50,SPHERE_N,SPHERE_N);
        glPopMatrix();
        
        setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.8,32);
        glPushMatrix();
        glTranslated(290,170,150);
        glutSolidSphere(50,SPHERE_N,SPHERE_N);
        glPopMatrix();
        
        setGlMaterial(1.0f,0.8f,0.0f,0.2,0.8,0.0,1);
        glPushMatrix();
        glTranslated(140,220,400);
        glutSolidSphere(50,SPHERE_N,SPHERE_N);
        glPopMatrix();
        
        setGlMaterial(1.0f,0.5f,0.0f,0.2,0.8,0.5,32);
        glPushMatrix();
        glTranslated(110,130,200);
        glutSolidSphere(50,SPHERE_N,SPHERE_N);
        glPopMatrix();
        
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
    double n = ((double)spiralIndex / (apertureSamples - 1)) * maximumN;
    double theta = n * 137.508;
    double radius = ATTENUATION_CONSTANT * sqrt(theta);
    
    double eyeDisplacementX = radius * cos(theta*PI/180);
    double eyeDisplacementY = radius * sin(theta*PI/180);
    
    lookAtPosition[0] = eyePosition[0] + sin(rotY*PI/180);
    lookAtPosition[1] = eyePosition[1] + cos(rotX*PI/180);
    lookAtPosition[2] = eyePosition[2] - cos(rotY*PI/180);
    
    gluLookAt(eyePosition[0] + eyeDisplacementX, eyePosition[1] + eyeDisplacementY,eyePosition[2],lookAtPosition[0],lookAtPosition[1],lookAtPosition[2],0.0f, 1.0f, 0.0f);
}

void computeMovement()
{
    if (wPressed)
    {
        eyePosition[0] += 0.5 * sin(rotY*PI/180);
        eyePosition[2] += 0.5 * -cos(rotY*PI/180);
    }
    if (sPressed)
    {
        eyePosition[0] -= 0.5 * sin(rotY*PI/180);
        eyePosition[2] -= 0.5 * -cos(rotY*PI/180);
    }
    if (aPressed)
    {
        // Not working correctly
        eyePosition[0] += 0.025 * cos(rotY*PI/180);
        eyePosition[2] += -0.025 * sin(rotY*PI/180);
    }
    if (dPressed)
    {
        // Also not working correctly
        eyePosition[0] -= 0.025 * cos(rotY*PI/180);
        eyePosition[2] -= -0.025 * sin(rotY*PI/180);
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
            spheresRotY += 2;
        }
        if (delta_x < 0) // Went left
        {
            spheresRotY -= 2;
        }
        if (delta_y > 0) // Went up
        {
            spheresRotX += 2;
        }
        if (delta_y < 0) // Went down
        {
            spheresRotX -= 2;
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
    gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,100,10000);
    glMatrixMode(GL_MODELVIEW);
    
    windowDimensions[0] = w;
    windowDimensions[1] = h;
}

void showStartMessage()
{
    puts("Computer Graphics Assignment - OpenGL:");
    puts("This is a simulation of the Phong Lighting Model");
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
