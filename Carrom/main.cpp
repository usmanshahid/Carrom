#include <iostream>
#include <Windows.h>
#include "gl/glut.h"
#include "SOIL.h"
#include "vec2d.h"
#include "disc.h"
#include "cloud.h"
#include "board.h"

using namespace std;

GLuint board_texture, disc_texture, disc_boundary_texture;
float k = 60;
float pwidth = 1280, pheight = 720;
float fps;
board carrom_board(4, board::POINT_CARROM);


const GLfloat high_shininess[] = {5.0f};
const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
const GLfloat light_diffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

const GLfloat center[] = { 0.0f, 0.0f, 0.0f};
const GLfloat front[] = {0.0f, 0.0f, 5.0f};
const GLfloat top[] = {0.0f, 0.0f, 15.0f};


//Initializes 3D rendering
void initialize()
{
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	load_textures();
}


//idle function
void advance(int)
{
	carrom_board.advance(dt);
	//glutPostRedisplay();
	glutTimerFunc(dt, advance, 0);
}


void idle(){
	
}


//keypress handling
void handle_keypress(unsigned char key, int x, int y)
{
    switch(key)
    {
		case 27:
			exit(0);
		case 'S':
		case 's':
			SOIL_save_screenshot(
				"awesomeness.bmp",
				SOIL_SAVE_TYPE_BMP,
				20, 20,1280, 720);
			break;

		case 'a':
		case'A':
			k++;
			break;
		case 'z':
		case 'Z':
			k--;
			break;
				
    }
    glutPostRedisplay();
}


//handle mouse
void handle_click(int button, int state, int x, int y) 
{
	/*float x1, y1;
	if ( button==GLUT_LEFT_BUTTON && state==GLUT_DOWN )
	{
		x1 = ((float)x/(float)pwidth)*w*2 - w;i
		y1 = h - ((float)y/(float)pheight)*h*2;
		cout << x1 << " " << y1 << endl;
		carrom_board.set_velocity(0, vec2d(vec2d(x1, y1), 20));
	}*/
	carrom_board.mouse_click(button, state, x, y);
}

void handle_move(int x, int y){
	carrom_board.mouse_move(x, y);
}


void handle_drag(int x, int y){
	carrom_board.mouse_drag(x, y);
}



//Called when the window is resized
void handle_resize(int width, int height)
{
	GLfloat aspectRatio;
	if(height == 0)
		height = 1;
	pwidth = width;
	pheight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectRatio = (GLfloat)width / (GLfloat)height;
	gluPerspective(45.0f, aspectRatio, 1.0, 425.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//Draws the 3D scene
void draw()
{
	static float prev_time = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	float current_time = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, top);
	glLoadIdentity();

	glColor3f(0, 1, 0);
	glTranslatef(0, 0, -10);
	glRotatef(k, -1, 0, 0);
	//glRotatef(k, 0, 0 , 1);
	carrom_board.draw();
	glutSwapBuffers();
	glutPostRedisplay();
	fps = 1.0/(current_time - prev_time);
	cout << fps << endl;
	prev_time = current_time;
}



//Main function
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(pwidth, pheight);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Carrom");
	initialize();
	glutIdleFunc(idle);
	glutDisplayFunc(draw);
	glutReshapeFunc(handle_resize);
	glutKeyboardFunc(handle_keypress);
	glutMouseFunc(handle_click);
	glutPassiveMotionFunc(handle_move);
	glutMotionFunc(handle_drag);
	//glutFullScreen();
	glutTimerFunc(dt, advance, 0);
	glutMainLoop();
	return 0;
}