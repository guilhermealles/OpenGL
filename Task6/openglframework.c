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
#define SPHERE_N (35)
#define NUMBER_OF_SPHERES 5

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lodepng.h"

GLuint windowDimensions[2] = { 800, 800 };

GLfloat eyePosition[3] = { 0, 0, 10 };
GLfloat lookAtPosition[3] = { 0, 0, 0 };

void initQuadric();
void setLight();
void showStartMessage();
void display(void);
void updateCamera();
void onKeyDown(unsigned char key, int x, int y);
void reshape(int w, int h);
void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n);
GLuint initTexture(char *filename);

GLUquadric *quadric;
GLuint texture[NUMBER_OF_SPHERES];
GLdouble rotationAngle = 0.0;
bool lastWasPlus = true;
GLdouble displacementX = 0.0;

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
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    /* Register GLUT callback functions */
    glutDisplayFunc(display);
    glutKeyboardFunc(onKeyDown);
    glutReshapeFunc(reshape);
    
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
    initQuadric();
    
    texture[0] = initTexture("textures/earth.png");
    texture[1] = initTexture("textures/texture1.png");
    texture[2] = initTexture("textures/texture2.png");
    texture[3] = initTexture("textures/texture3.png");
    texture[4] = initTexture("textures/texture4.png");
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    showStartMessage();

    glutMainLoop();
    return 0;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f,0.0f,1.0f);
    glLoadIdentity();
    
    updateCamera();
    setLight();
    
    
    setGlMaterial(1.0f,1.0f,1.0f,1.0,0.8,0.1,64);
    glPushMatrix();
    glTranslated(0.0+displacementX, 0.0, -20.0);
    glRotated(rotationAngle, 1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    gluSphere(quadric, 2,SPHERE_N,SPHERE_N);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    
    setGlMaterial(1.0f,1.0f,1.0f,1.0,0.8,0.1,64);
    glPushMatrix();
    glTranslated(0.0+displacementX, 5.0, -20.0);
    glRotated(rotationAngle, 1.0,-1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    gluSphere(quadric, 2,SPHERE_N,SPHERE_N);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    
    setGlMaterial(1.0f,1.0f,1.0f,1.0,0.8,0.1,64);
    glPushMatrix();
    glTranslated(0.0+displacementX, -5.0, -20.0);
    glRotated(rotationAngle, -1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    gluSphere(quadric, 2,SPHERE_N,SPHERE_N);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    
    setGlMaterial(1.0f,1.0f,1.0f,1.0,0.8,0.1,64);
    glPushMatrix();
    glTranslated(5.0+displacementX, 0.0, -20.0);
    glRotated(rotationAngle, 1.0,1.0,-1.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    gluSphere(quadric, 2,SPHERE_N,SPHERE_N);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    
    setGlMaterial(1.0f,1.0f,1.0f,1.0,0.8,0.1,64);
    glPushMatrix();
    glTranslated(-5.0+displacementX, 0.0, -20.0);
    glRotated(rotationAngle, -1.0,-1.0,-1.0);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[4]);
    gluSphere(quadric, 2,SPHERE_N,SPHERE_N);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    rotationAngle++;
    if (lastWasPlus) {
        displacementX+= 0.1;
    }
    else {
        displacementX-=0.1;
    }
    if (displacementX >= 2.5 && lastWasPlus) {
        lastWasPlus = false;
    }
    else if (displacementX >= 2.5 && !lastWasPlus) {
        displacementX-=0.1;
    }
    else if (displacementX <= -2.5 && !lastWasPlus) {
        lastWasPlus = true;
    }
    else if (displacementX <= -2.5 && lastWasPlus) {
        displacementX +=0.1;
    }
    
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void initQuadric() {
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
}

void setLight()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat mat_ambient[] = { 1.0,1.0,1.0,1.0 };
    GLfloat mat_diffuse[] = { 1.0,1.0,1.0,1.0 };
    GLfloat mat_specular[] = {1.0,1.0,1.0,1.0};
    GLfloat light_position[] = {0, 7, -5, 1.0f};
    glClearColor (0.0,0.0,0.0,0.0);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
}

void updateCamera()
{
    gluLookAt(eyePosition[0],eyePosition[1],eyePosition[2],lookAtPosition[0],lookAtPosition[1],lookAtPosition[2],0.0f, 1.0f, 0.0f);
}

void onKeyDown(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':
        case 'Q':
        case 27: // ESC key
            printf("Exiting...\n");
            exit(0);
            break;
    }
}

void reshape(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,2,500);
    glMatrixMode(GL_MODELVIEW);
    
    windowDimensions[0] = w;
    windowDimensions[1] = h;
}

void showStartMessage()
{
    puts("Computer Graphics Assignment - OpenGL:");
    puts("This is a simple animation");
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

GLuint initTexture(char *filename) {
    unsigned char* buffer;
    unsigned char* image;
    size_t buffersize, imagesize;
    GLuint texName;
    LodePNG_Decoder decoder;
    
    LodePNG_loadFile(&buffer, &buffersize, filename);
    LodePNG_Decoder_init(&decoder);
    decoder.infoRaw.color.colorType = 6; /* Load image as RGBA */
    LodePNG_decode(&decoder, &image, &imagesize, buffer, buffersize);
    if(decoder.error) {
 	      printf("Error reading in png image: %d\n", decoder.error);
 	      exit(1);
    } else {
 	      glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 	      glGenTextures(1,&texName);
 	      glBindTexture(GL_TEXTURE_2D,texName);
 	      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
 	      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
 	      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 	      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 	      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,decoder.infoPng.width,
                       decoder.infoPng.height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    }
    return texName;
}
