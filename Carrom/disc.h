#pragma once


#include "vec3d.h"
#include "constants.h"
#include <string>

class vec2d;
class disc_engine;

class disc
{
public:
	//private data members
	float mass;
	float rad;
	vec2d velocity;
	vec2d position;
	vec3d color;
	float transparency;
	string type;

	//constructor with some default values
	//order is (mass, radius, position, velocity)
	disc(float = 1.0, float = 1.0, vec2d = vec2d(0, 0), vec2d = vec2d(0, 0), vec3d = vec3d(1, 1, 1), float = 1.0);

	//to advance the state of disc by time t(in ms)
	bool advance(float);

	//to check collisions
	bool colliding_with(const disc&);

	//to handle collisions
	void handle_collision(disc&);

	//to bounce back off wall
	void bounce_back(DIRECTION);

	//Function to draw a disk
	void draw();

	friend class disc_engine;
};

