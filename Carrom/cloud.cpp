#include <Windows.h>
#include "gl/glut.h"
#include <math.h>

#include "vec2d.h"
#include "constants.h"
#include "cloud.h"
#include "SOIL.h"

//function to draw a filled circle
void filled_circle(float rad, float start, float end){
	if(start < end){
		float segments = 20;
		end = end + PI/(segments - 1);
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0, 0, 1);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(0, 0, 0);
			for(double i = start; i <= end; i+=PI/segments)
			{
				glTexCoord2f(0.5 + (0.5*cos(i)), 0.5+(0.5*sin(i)));
				glVertex3f(rad*cos(i), rad*sin(i), 0);
			}
		glEnd();
	}
}


//function to draw a circle
void circle(float rad)
{
	glPushMatrix();
		glBegin(GL_LINE_LOOP);
			for(double i = 0; i < 2*PI; i+=PI/720)
			{
				glVertex2f(rad*cos(i), rad*sin(i));
			}
		glEnd();
	glPopMatrix();
}


void open_cylinder(vec3d start, vec3d normal, float radius, float length){
	glPushMatrix();
		glTranslatef(start.x, start.y, start.z);

		if(normal.mag() == 0)
			normal = vec3d(0, 1, 0);

		//Rotating orbit w.r.t normal
		vec3d normal_vec = normal.cross(vec3d(0, 0, -1));
		float ang = normal.angle(vec3d(0, 0, -1));
		glRotatef(ang * 180 / PI,  normal_vec.x, normal_vec.y, normal_vec.z);
		glBegin(GL_QUAD_STRIP);
			for(double i = 0; i <= 2*PI + 0.1; i+=PI/10)
			{
					glNormal3f(cos(i), sin(i), 0);
					glVertex3f(radius*cos(i), radius*sin(i), 0);
					glVertex3f(radius*cos(i), radius*sin(i), length);
			}
		glEnd();
	glPopMatrix();
}


void disk(float rad)
{
	//upper circle
	glBegin(GL_TRIANGLE_FAN);
		for(double i = 0; i < 2*PI; i+=PI/60)
		{
			glVertex2f(rad*cos(i), rad*sin(i));
		}
	glEnd();

	//lower circle
	glBegin(GL_TRIANGLE_FAN);
		for(double i = 0; i < 2*PI; i+=PI/60)
		{
			glVertex3f(rad*cos(i), rad*sin(i), -0.05 );
		}
	glEnd();

	//boundary
	glBegin(GL_QUADS);
		for(double i = 0; i < 2*PI; i+=PI/60)
		{
			glVertex3f(rad*cos(i), rad*sin(i), 0.0);
			glVertex3f(rad*cos(i), rad*sin(i), -0.05);
		}
	glEnd();
}

double distance2d(const vec2d& p1, const vec2d& p2)
{
	return sqrt((p1.x-p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

double distance3d(const vec3d& p1, const vec3d& p2)
{
	return sqrt((p1.x-p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z));
}



vec2d get_mapped_coordinates(const vec2d& tex_coord, const vec2d& tex_dim, const vec2d& world_dim)
{
	vec2d half = tex_dim/2;
	return vec2d((tex_coord.x - half.x)/half.x * world_dim.x, (half.y - tex_coord.y)/half.y * world_dim.y);
}


//function to return sign
int sign(double val){
	if(val < 0)
		return -1;
	return 1;
}


void load_textures(){
	
	// load an image file directly as a new OpenGL texture
	board_texture = SOIL_load_OGL_texture
	(
		"carrom_board.png",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA 
	);
	
	// load an image file directly as a new OpenGL texture
	disc_texture = SOIL_load_OGL_texture
	(
		"disc_texture.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
	);

	// load an image file directly as a new OpenGL texture
	disc_boundary_texture = SOIL_load_OGL_texture
	(
		"disc_side_texture.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
	);

	cout << board_texture << endl;
}