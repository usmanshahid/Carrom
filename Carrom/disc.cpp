#include <windows.h>
#include "gl/glut.h"

#include "disc.h"
#include "cloud.h"

//constructor for disc
disc :: disc(float _mass, float _rad, vec2d _position, vec2d _velocity, vec3d _color, float _transparency) : mass(_mass), rad(_rad), position(_position), velocity(_velocity), color(_color), transparency(_transparency)
{}


//to advance the state of disc by time t(in ms)
bool disc :: advance(float t)
{
	float mag = velocity.mag();
	
	if(velocity.mag()<0.05)
	{
		velocity = vec2d(0, 0);
		return false;
	}
	
	//adding friction
	velocity = velocity + (velocity.unit()*(-0.02));
	
	position = position + velocity*(t/1000.0);
	return true;
}


//to check collisions
bool disc :: colliding_with(const disc& _disc)
{
	if(rad + _disc.rad < distance2d(position, _disc.position))
		return false;
	else
		return true;
}


//to handle collisions
void disc :: handle_collision(disc& _disc)
{
	advance(-dt);
	_disc.advance(-dt);
	
	vec2d collision_dir = (_disc.position - position).unit();
	double angle = collision_dir.angle(vec2d(1, 0));
	
	// Unit vector in the direction of the collision
	double ax=collision_dir.unit().x, ay=collision_dir.unit().y; 

	double vx1 = velocity.x, vy1 = velocity.y;
	double vx2 = _disc.velocity.x, vy2 = _disc.velocity.y;

	// Projection of the velocities in these axes
	double va1=(vx1*ax+vy1*ay), vb1=(-vx1*ay+vy1*ax); 
	double va2=(vx2*ax+vy2*ay), vb2=(-vx2*ay+vy2*ax);

	// New velocities in these axes (after collision): ed<=1,  for elastic collision ed=1
	double vaP1=va1 + (2)*(va2-va1)/(1+mass/_disc.mass);
	double vaP2=va2 + (2)*(va1-va2)/(1+_disc.mass/mass);
	
	// Undo the projections
	vx1=vaP1*ax-vb1*ay;  vy1=vaP1*ay+vb1*ax;// new vx,vy for ball 1 after collision
	vx2=vaP2*ax-vb2*ay;  vy2=vaP2*ay+vb2*ax;// new vx,vy for ball 2 after collision

	velocity = vec2d(vx1, vy1);
	_disc.velocity = vec2d(vx2, vy2);

	advance(dt);
	_disc.advance(dt);
}


//to bounce back off the wall
void disc :: bounce_back(DIRECTION wall)
{
	advance(-dt);
	switch(wall)
	{
	case UP:
		velocity.y = abs(velocity.y) * -1;
		break;
	case DOWN:
		velocity.y = abs(velocity.y);
		break;
	case RIGHT:
		velocity.x = abs(velocity.x) * -1;
		break;
	case LEFT:
		velocity.x = abs(velocity.x);
		break;
	}
	advance(dt);
}


//Function to draw a disk
void disc :: draw()
{
	glColor4f(color.x, color.y, color.z, transparency);
	glBindTexture(GL_TEXTURE_2D, disc_texture);
	glPushMatrix();
		glTranslatef(position.x, position.y, 0);

		//upper circle
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0, 0, 1);
		
			for(double i = 0; i <= 2*PI; i+=PI/10)
			{
				glTexCoord2f(0.5 + (0.5*cos(i)), 0.5+(0.5*sin(i)));
				glVertex3f(rad*cos(i), rad*sin(i), 0.07);
			}
		glEnd();

		//lower circle
		glBegin(GL_TRIANGLE_FAN);
			for(double i = 0; i <= 2*PI; i+=PI/10)
			{	
				glNormal3f(0, 0, -1);
				glVertex3f(rad*cos(i), rad*sin(i), 0.01);
			}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, disc_boundary_texture);
		bool sw = false;
		//boundary
		glBegin(GL_QUAD_STRIP);
			for(double i = 0; i <= 2*PI+0.1; i+=PI/10)
			{
					glNormal3f(-cos(i), -sin(i), 0);
					glTexCoord2d(sw, 1);
					glVertex3f(rad*cos(i), rad*sin(i), 0.06);
					glTexCoord2d(sw, 0);
					glVertex3f(rad*cos(i), rad*sin(i), 0.01);
					sw = !sw;

			}
		glEnd();
		
	glPopMatrix();
}

