#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/glut.h>
#include <cmath>

using namespace std;

const int screenWidth = 640;
const int screenHeight = 480;



//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
glClearColor(1.0,1.0,1.0,0.0);       // set white background color
glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color
glPointSize(4.0);       // a ‘dot’ is 4 by 4 pixels
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
}
//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
glClear(GL_COLOR_BUFFER_BIT);     // clear the screen

// Write your drawing code here

glFlush();                 // send all output to display
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int  main(int argc, char** argv)
{
glutInit(&argc, argv);          // initialize the toolkit
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
glutInitWindowSize(screenWidth,screenHeight);     // set window size
glutInitWindowPosition(100, 100); // set window position on screen
glutCreateWindow("My Graphics App"); // open the screen window
glutDisplayFunc(myDisplay);     // register redraw function
myInit();
glutMainLoop();      // go into a perpetual loop

return 0;
}
