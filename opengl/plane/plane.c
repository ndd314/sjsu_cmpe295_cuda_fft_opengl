/*-----------------------------------------------------------*
 * plane.c                                                   *
 * Sample OpenGL application to draw a plane with texture.   *
 * Author: MH                                                *
 * Updated by: HL             Date: Sept. 25, 2014;          *
 * Note:                                                     *
 *      compile and build                                    *
 *      g++   -lGL -lglut -lGLU -lm                          *
 *-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GLUT/glut.h>
#include "readBMP.h"

#define PI 3.141592653589793238462643

void display(void);
void createWorld(void);
void createObjects(void);
void createLighting(void);
void createCamera(void);
void drawAxes(GLfloat length);
void drawPlane(GLfloat width, GLfloat height);
void handleKeyboard(unsigned char key, int x, int y);
void handleSpecialKeyboard(int key, int x, int y);
void handleVisibility(int visible);
void handleIdle(void);
void printUsage(void);
int  LoadBmpTexture(char * filename, GLenum minFilter, GLenum magFilter, GLenum wrapMode);

// Globals
char *textureFile = "grape12.bmp";
GLuint textureID;
double verticalRotation = 60;
double horizontalRotation = 0;

/*----------------------------------------------*
 * main
 * Main entry function for the application.
 *----------------------------------------------*/
int main(int argc, char **argv)
{
    int i;

    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-t") == 0)
        {
            if(i+1 >= argc)
                printUsage();
            textureFile = argv[i+1];
        }
    }

    // OpenGL initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("OpenGL: Textured Plane");
    glutDisplayFunc(display);
    glutVisibilityFunc(handleVisibility);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(handleSpecialKeyboard);

    // Create world
    createWorld();

    // main loop
    glutMainLoop();
    return 0;
}

/**
 * createWorld
 * Creates the world and its objects.
 */
void createWorld(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);

    glLineWidth(1.0);
    glPointSize(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFrontFace(GL_CW);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_COLOR_MATERIAL);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    LoadBmpTexture(textureFile, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

/**
 * display
 * This is the display function. Creates lighting, camera, and geometry.
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    createCamera();
    createLighting();
    createObjects();
    glPopMatrix();

    glutSwapBuffers();
}

/**
 * createObjects
 * Creates the world objects.
 */
void createObjects(void)
{
   drawAxes(500);

   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, textureID);
   drawPlane(100.0, 100.0);
   glDisable(GL_TEXTURE_2D);
}

/**
 * drawAxes
 * Draws the x, y, and z axes.
 */
void drawAxes(GLfloat length)
{
    int i;
    // axes
    GLfloat origin[3] = {0, 0, 0};
    GLfloat xpos[3] = { 1,  0,  0};
    GLfloat xneg[3] = {-1,  0,  0};
    GLfloat ypos[3] = { 0,  1,  0};
    GLfloat yneg[3] = { 0, -1,  0};
    GLfloat zpos[3] = { 0,  0,  1};
    GLfloat zneg[3] = { 0,  0, -1};

    glLineWidth(2.0);
    glBegin(GL_LINES);
    // x-axis (red)
    glColor3f(1, 0, 0);
    glVertex3fv(origin);
    glVertex3f(xpos[0] * length,
               xpos[1] * length,
               xpos[2] * length);
    glVertex3fv(origin);
    glVertex3f(xneg[0] * length,
               xneg[1] * length,
               xneg[2] * length);
    // y-axis (green)
    glColor3f(0, 1, 0);
    glVertex3fv(origin);
    glVertex3f(ypos[0] * length,
               ypos[1] * length,
               ypos[2] * length);
    glVertex3fv(origin);
    glVertex3f(yneg[0] * length,
               yneg[1] * length,
               yneg[2] * length);
    // z-axis (blue)
    glColor3f(0, 0, 1);
    glVertex3fv(origin);
    glVertex3f(zpos[0] * length,
               zpos[1] * length,
               zpos[2] * length);
    glVertex3fv(origin);
    glVertex3f(zneg[0] * length,
               zneg[1] * length,
               zneg[2] * length);
    glEnd();
}

/**
 * drawPlane
 * Draws a plane with the given dimensions.
 */
void drawPlane(GLfloat width, GLfloat height)
{
    int i;
    GLfloat normal[3] = {-1.0, 0.0, 0.0};
    GLfloat verts[4][3] = {
        {-1.0, -1.0, 0.0},
        { 1.0, -1.0, 0.0},
        { 1.0,  1.0, 0.0},
        {-1.0,  1.0, 0.0}
    };
    GLfloat texcoords[4][3] = {
        { 0.0,  0.0, 0.0},
        { 1.0,  0.0, 0.0},
        { 1.0,  1.0, 0.0},
        { 0.0,  1.0, 0.0}
    };

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    for(i = 0; i < 4; i++)
    {
        verts[i][0] *= width;
        verts[i][1] *= height;

        glNormal3fv(&normal[0]);
        glTexCoord3fv(&texcoords[i][0]);
        glVertex3fv(&verts[i][0]);
    }
    glEnd();
}

/**
 * createLighting
 * Creates the lighting for the scene.
 */
void createLighting(void)
{
    GLfloat ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat pos0[] = {1.0, 1.0, 0.0, 0.0};
    GLfloat dif0[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat pos1[] = {5.0, -5.0, 0.0, 0.0};
    GLfloat dif1[] = {0.4, 0.4, 0.4, 1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

/**
 * createCamera
 * Creates the camera for the scene.
 */
void createCamera(void)
{
    GLint viewport[4];

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0,    // field of view
                   1.0,     // aspect ratio
                   0.1,     // near
                   1000.0); // far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(300 * cos(horizontalRotation * PI/180) * sin(verticalRotation * PI / 180),
              300 * cos(verticalRotation * PI/180),
              300 * sin(horizontalRotation * PI/180) * sin(verticalRotation * PI / 180),
              0.0, 0.0, 0.0,    // focus
              0.0, 1.0, 0.0);   // up

    horizontalRotation += 0.2;
}

/**
 * handleKeyboard
 * Handles keyboard input.
 */
void handleKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: // escape key
        case 'Q':
        case 'q':
            exit(0);
            break;

        default:
            // nothing
            break;
    }
}

/**
 * handleSpecialKeyboard
 * Handles special keys from the keyboard.
 */
void handleSpecialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            verticalRotation -= 2;
            break;

        case GLUT_KEY_DOWN:
            verticalRotation += 2;
            break;
    }
}

/**
 * handleVisibility
 * Handle visibility changes.
 */
void handleVisibility(int visible)
{
    if(visible == GLUT_VISIBLE)
    {
        glutIdleFunc(handleIdle);
    }
    else
    {
        glutIdleFunc(NULL);
    }
}

/**
 * handleIdle
 * Handles idle events.
 */
void handleIdle(void)
{
    glutPostRedisplay();
}

/**
 * printUsage
 * Prints the program usage to the user.
 */
void printUsage(void)
{
    printf("Usage: plane [-t <texturefile>]\n");
    printf("     -t <texturefile>    Filename for the texture.\n");
    printf("Controls:\n");
    printf("     q: Quit\n");
    printf("  left: Rotate left\n");
    printf(" right: Rotate right\n");
    printf("    up: Rotate up\n");
    printf("  down: Rotate down\n");
    exit(0);
}


/* Reads a long 32 bit integer; comment out one or the other shifting line below,
whichever makes your system work right. */
unsigned int endianReadInt(FILE* file) {
unsigned char  b[4];
unsigned int i;

   if ( fread( b, 1, 4, file) < 4 )
     return 0;
   /* LITTLE VS BIG ENDIAN LINES - comment out one or the other */
   i = (b[3]<<24) | (b[2]<<16) | (b[1]<<8) | b[0]; // big endian
   //i = (b[0]<<24) | (b[1]<<16) | (b[2]<<8) | b[3]; // little endian
   return i;
}

/* Reads a 16 bit integer; comment out one or the other shifting line below,
whichever makes your system work right. */
unsigned short int endianReadShort(FILE* file) {
unsigned char  b[2];
unsigned short s;

   if ( fread( b, 1, 2, file) < 2 )
     return 0;
   /* LITTLE VS BIG ENDIAN LINES - comment out one or the other */
   s = (b[1]<<8) | b[0]; // big endian
   //s = (b[0]<<8) | b[1]; // little endian
   return s;
}

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
    printf("File Not Found : %s\n",filename);
    return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if (!(image->sizeX = endianReadInt(file))) {
    printf("Error reading width from %s.\n", filename);
    return 0;
    }
    printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if (!(image->sizeY = endianReadInt(file))) {
    printf("Error reading height from %s.\n", filename);
    return 0;
    }
    printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if (!(planes=endianReadShort(file))) {
    printf("Error reading planes from %s.\n", filename);
    return 0;
    }
    if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
    }

    // read the bits per pixel
    if (!(bpp = endianReadShort(file))) {
    printf("Error reading bpp from %s.\n", filename);
    return 0;
    }
    if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
    printf("Error allocating memory for color-corrected image data");
    return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
    }

    // we're done.
    return 1;
}

/**
 * LoadBmpTexture
 * Loads a texture from a BMP file.
 */
static Image * image = NULL;
int LoadBmpTexture(char * filename, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
    // read the texture bits
    if(image == NULL)
    {
        image = (Image *) malloc(sizeof(Image));
        if(image == NULL)
        {
            fprintf(stderr, "Error allocating memory for image.\n");
            return 0;
        }

        if(!ImageLoad(filename, image))
        {
            fprintf(stderr, "Error loading image: %s\n", filename);
            return 0;
        }
    }

    // set wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    // set filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);

/*
    // generate mipmap (if necessary)
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        magFilter == GL_LINEAR_MIPMAP_LINEAR ||
        magFilter == GL_LINEAR_MIPMAP_NEAREST)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
*/
    return 1;
}
