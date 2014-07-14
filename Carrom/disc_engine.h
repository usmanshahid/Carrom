#pragma once

#include <vector>
#include "disc.h"

using namespace std;

class disc_engine
{
	float x_limit, y_limit;

	//function to handle collisions
	void handle_disc_collisions();
	
	//function to handle wall collisions
	void handle_wall_collisions();
	
	//function to handle collisions
	void handle_collisions();

protected:
	vector<disc> discs;

public:

	enum state{STATIC, DYNAMIC};

	//constructor to initialize engine, takes the dimensions and count of discs
	disc_engine(float, float, int=0);

	//Advance state of system
	state advance(float);

	//Draw engine
	void draw();

	//set velocity
	bool set_velocity(unsigned int, const vec2d&);

	//get state of the engine

	//rule to sink a disc
	bool sink();

	//function to test if a disc can be added to system or not
	bool can_add(const disc& _disc);

	//Every time the disc is added
	bool add_disc(const disc&);

	//function to remove disc
	bool remove_disc(unsigned int);
};
