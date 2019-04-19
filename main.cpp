#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/glut.h>
#include <cmath>

using namespace std;

const int screenWidth = 800;
const int screenHeight = 600;

class Ball {
public:
	float PosX, PosY, VelX, VelY;
	float radius = 10; //Feel free to play around with the radius of the ball
	float color[3]; //Useless for now

	void draw(); //Dont use this function since its embedded into move();
	void bounce(float, float); //pass a vector here
	void move(); //moves the ball according to its velocity

	Ball(float, float, float, float);
};

Ball::Ball(float pX, float pY, float vX, float vY) {
	PosX = pX;
	PosY = pY;
	VelX = vX;
	VelY = vY;
}

class Brick {
public:
	float PosX, PosY;
	float width = 60.0, height = 20.0; //Feel free to mess around with it
	float color[3]; //Useless for now
	bool isDestroy = false;

	void draw();
	void collision(Ball&); //Checks if the brick is colliding with the ball and if so it destroys the brick and reflects the ball

	Brick(); //We need this so we can make an array
	Brick(float, float); //Easily create the brick by passing the x and y value of where the center of the brick is.
};

Brick::Brick() {
	PosX = 0;
	PosY = 0;
}

Brick::Brick(float x, float y) {
	PosX = x;
	PosY = y;
}

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

glutSwapBuffers();
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

void Brick::collision(Ball &ball) {
	if (isDestroy == false) {
		float corner[4][2];

		corner[0][0] = PosX - width / 2;
		corner[0][1] = PosY - height / 2;

		corner[1][0] = PosX - width / 2;
		corner[1][1] = PosY + height / 2;

		corner[2][0] = PosX + width / 2;
		corner[2][1] = PosY + height / 2;

		corner[3][0] = PosX + width / 2;
		corner[3][1] = PosY - height / 2;

		float minimum = 2 * ball.radius;

		float normVector[2];

		for (int i = 0; i < 4; i++) {
			double vector[2];

			vector[0] = corner[(i + 1) % 4][0] - corner[i][0];
			vector[1] = corner[(i + 1) % 4][1] - corner[i][1];

			double t = (ball.PosX*vector[0]+ball.PosY*vector[1] - corner[i][0]*vector[0]-corner[i][1]*vector[1]) / (vector[0] * vector[0] + vector[1] * vector[1]);
			float nearestPoint[2];

			nearestPoint[0] = corner[i][0] + t * vector[0];
			nearestPoint[1] = corner[i][1] + t * vector[1];

			float perpVector[2];

			perpVector[0] = nearestPoint[0] - ball.PosX;
			perpVector[1] = nearestPoint[1] - ball.PosY;

			float distance = sqrt(perpVector[0] * perpVector[0] + perpVector[1] * perpVector[1]);

			if (distance < minimum && t<=1 && t>=0) {
				minimum = distance;
				normVector[0] = perpVector[0];
				normVector[1] = perpVector[1];
			}
		}

		if (minimum <= ball.radius) {

			ball.VelX = ball.VelX - 2 * (ball.VelX*normVector[0] + ball.VelY * normVector[1])*normVector[0] / (normVector[1] * normVector[1] + normVector[0] * normVector[0]);
			ball.VelY = ball.VelY - 2 * (ball.VelX*normVector[0] + ball.VelY * normVector[1])*normVector[1] / (normVector[1] * normVector[1] + normVector[0] * normVector[0]);

			cout << ball.VelY;

			isDestroy = !isDestroy;
		}
	}
}

void Ball::bounce(float x, float y) {
	float normX = -y;
	float normY = x;

	VelX = VelX - 2 * (VelX*normX + VelY * normY)*normX;
	VelY = VelY - 2 * (VelX*normX + VelY * normY)*normY;
}

void Ball::draw(){
	glColor3f(1.0, 0, 0);

	glBegin(GL_POLYGON);
	for (float t = 0; t < 1; t+=0.05) {
		glVertex2i(PosX + radius * cos(2 * 3.141592*t), PosY + radius * sin(2 * 3.141592*t));
	}
	glEnd();
}

void Brick::draw() {
	if (isDestroy == false) {
		glBegin(GL_QUADS);
		glVertex2i(PosX + width / 2, PosY + height / 2);
		glVertex2i(PosX + width / 2, PosY - height / 2);
		glVertex2i(PosX - width / 2, PosY - height / 2);
		glVertex2i(PosX - width / 2, PosY + height / 2);
		glEnd();
		glutPostRedisplay();
	}
	else {

	}
}

void Ball::move() {
	PosX = PosX + VelX;
	PosY += VelY;
	
	draw();
	glutPostRedisplay();
}