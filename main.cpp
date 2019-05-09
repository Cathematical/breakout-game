#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/glut.h>
#include <cmath>
#include <ctime>

#define lvl1 1
#define lvl2 2
#define lvl3 3


using namespace std;

const int screenWidth = 800;
const int screenHeight = 600;
int moveHX =screenWidth/2;
double speed = 0.5;

int menupick=1;

void LevelPick(int);
int const row = 13, col = 11;

class Ball {
public:
	double PosX, PosY, VelX, VelY;
	float radius = 10;
	float color[3];

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
	float width = 60.0, height = 20.0;
	float color[3]; //Useless for now
	bool isDestroy = false;

	void draw();
	void collision(Ball&); //Checks if the brick is colliding with the ball and if so it destroys the brick and reflects the ball
    void setPos(float, float);

	Brick(); //We need this so we can make an array
};
Brick::Brick() {
	PosX = 0;
	PosY = screenHeight/2-10;
    color[0] = (rand()%255)/255.0;
    color[1] = (rand()%255)/255.0;
    color[2] = (rand()%255)/255.0;
}

void Brick::setPos(float x, float y) {
	PosX = x;
	PosY = y;
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

            if(speed<=.75){
                speed+=0.05;
            }

			ball.VelX = ball.VelX - 2 * (ball.VelX*normVector[0] + ball.VelY * normVector[1])*normVector[0] / (normVector[1] * normVector[1] + normVector[0] * normVector[0]);
			ball.VelY = ball.VelY - 2 * (ball.VelX*normVector[0] + ball.VelY * normVector[1])*normVector[1] / (normVector[1] * normVector[1] + normVector[0] * normVector[0]);
            ball.color[0] = color[0];
            ball.color[1] = color[1];
            ball.color[2] = color[2];

            double ballspeed;

            ballspeed = pow(ball.VelX*ball.VelX + ball.VelY*ball.VelY,0.5);

            ball.VelX = ball.VelX/ballspeed * speed;
            ball.VelY = ball.VelY/ballspeed * speed;

			isDestroy = !isDestroy;
		}
	}
}

void Ball::bounce(float x, float y) {
	float normX = -y;
	float normY = x;

	VelX = VelX - 2 * (VelX*normX + VelY * normY)*normX / (normX*normX+normY*normY);
	VelY = VelY - 2 * (VelX*normX + VelY * normY)*normY/ (normX*normX+normY*normY);

	double ballspeed;

    ballspeed = pow(VelX*VelX + VelY*VelY,0.5);

    VelX = VelX/ballspeed * speed;
    VelY = VelY/ballspeed * speed;
}

void Ball::draw(){
	glColor3f(color[0], color[1], color[2]);

	glBegin(GL_POLYGON);
	for (float t = 0; t < 1; t+=0.05) {
		glVertex2i(PosX + radius * cos(2 * 3.141592*t), PosY + radius * sin(2 * 3.141592*t));
	}
	glEnd();
}

void Brick::draw() {
	if (isDestroy == false) {
        glColor3f(color[0],color[1],color[2]);
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

void myDisplay(void);
void myKeyboard(unsigned char key, int x, int y);
void myInit(void);
void processMenuEvents(int);

//Ball

Ball ballObj = Ball(screenWidth/2, screenHeight/2, 0.1,-0.9949);

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int  main(int argc, char** argv)
{
srand(time(0));
glutInit(&argc, argv);          // initialize the toolkit
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
glutInitWindowSize(screenWidth,screenHeight);     // set window size
glutInitWindowPosition(100, 100); // set window position on screen
glutCreateWindow("Acne BreakOut"); // open the screen window
glutDisplayFunc(myDisplay);     // register redraw function
glutKeyboardFunc(myKeyboard);
glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Level 1", lvl1);
	glutAddMenuEntry("Level 2", lvl2);
	glutAddMenuEntry("Level 3", lvl3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

myInit();
glutMainLoop();      // go into a perpetual loop


return 0;
}

//Bricks
Brick brickObj[row][col];

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{

glClearColor(1.0,1.0,1.0,0.0);       // set white background color
glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color
glPointSize(4.0);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);

ballObj= Ball(screenWidth/2, screenHeight/2, 0.25,-0.75);

double ballspeed;

ballspeed = pow(ballObj.VelX*ballObj.VelX + ballObj.VelY*ballObj.VelY,0.5);

ballObj.VelX = ballObj.VelX/ballspeed * speed;
ballObj.VelY = ballObj.VelY/ballspeed * speed;

for(int i = 0; i<row; i++)
    for(int j = 0; j<col; j++)
        brickObj[i][j]= Brick();

}

///Levels are drawn here, moved the drawing section here
void processMenuEvents(int option )
 {
	 switch (option)
	 {
	     case lvl1:
	     for(int i = 0; i<row; i++)
            for(int j = 0; j<col; j++)
                brickObj[i][j].setPos(50+70*j,40*i+100); ///must fix here

	     ///level 1
        for(int i = 0; i<5; i++)
            for(int j = 0; j<col; j++)
                brickObj[i][j].isDestroy=true;
        ///level 1 ends here ;
        break;

        case lvl2:
            for(int i = 0; i<row; i++)
                for(int j = 0; j<col; j++)
                brickObj[i][j].setPos(50+70*j,40*i+100);
            ///level H, level 2
            for(int i = 0; i<4; i++)
                for(int j = 3; j<8; j++)
                brickObj[i][j].isDestroy=true;
            for(int i = 12; i>8; i--)
                for(int j = 3; j<8; j++)
                brickObj[i][j].isDestroy=true ;
        break;

        case lvl3:
	     for(int i = 0; i<row; i++)
            for(int j = 0; j<col; j++)
                brickObj[i][j].setPos(50+70*j,40*i+100);
        break;
	 }
 }

void myDisplay(void)
{
glClear(GL_COLOR_BUFFER_BIT);     // clear the screen

glColor3d(1.0, 0.0, 1.0);

/// draw the bar
glBegin(GL_QUADS);
	glVertex2i(moveHX, 30);
	glVertex2i(moveHX, 20);
	glVertex2i(moveHX+100, 20);
	glVertex2i(moveHX+100, 30);
glEnd();

for(int i= 0; i<row; i++)
    for(int j=0; j<col; j++)
        brickObj[i][j].draw();

ballObj.move();

for(int i= 0; i<row; i++)
    for(int j=0; j<col; j++)
        brickObj[i][j].collision(ballObj);

if(ballObj.PosY>=screenHeight)
    ballObj.bounce(1,0);

if(ballObj.PosY<=15){
    ballObj.PosX = screenWidth/2;
    ballObj.PosY = screenHeight/2;
    ballObj.VelX = 0;
    ballObj.VelY = 0;

}

if(ballObj.PosX>=screenWidth)
    ballObj.bounce(0,1);
if(ballObj.PosX<=0)
    ballObj.bounce(0,1);

///ball object bouncing off the bar
if(ballObj.PosX+ballObj.radius/2>moveHX && ballObj.PosX-ballObj.radius/2<moveHX+100 && ballObj.PosY-ballObj.radius<=30){
        ballObj.PosY = 30+ballObj.radius;
        ballObj.bounce(1,0);
}


glutSwapBuffers();
glFlush();                 // send all output to display
}

void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'A' && moveHX>=0)
	{
		moveHX = moveHX - 50;
	}

	if (key == 'd' || key == 'D' && moveHX<=screenWidth-100)
	{
		moveHX = moveHX + 50;
	}

	if(key == 'r' || key == 'R'){
        myInit();
	}

}
