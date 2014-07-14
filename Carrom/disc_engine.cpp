#include <Windows.h>
#include "gl/glut.h"


#include "disc_engine.h"
#include "cloud.h"

//function to handle collisions
void disc_engine :: handle_disc_collisions()
{
	for(vector<disc> :: iterator i = discs.begin(); i < discs.end(); i++)
	{
		for(vector<disc> :: iterator j = i+1; j < discs.end(); j++)
		{
			if(i->colliding_with(*j))
				i->handle_collision(*j);
		}
	}
}


//function to handle wall collisions
void disc_engine :: handle_wall_collisions()
{	
	for(vector<disc> :: iterator i = discs.begin(); i < discs.end(); i++)
	{
		float x = i->position.x, y = i->position.y;
		float r = i->rad;
		if(x > x_limit-r)
		{
			i->bounce_back(RIGHT);
		}
		else if( x < -x_limit + r)
		{
			i->bounce_back(LEFT);
		}
		if(y > y_limit - r)
		{
			i->bounce_back(UP);
		}
		else if(y < -y_limit + r)
		{
			i->bounce_back(DOWN);
		}
	}
}


//function to handle collisions
void disc_engine :: handle_collisions()
{
	handle_wall_collisions();
	handle_disc_collisions();
}


//constructor to initialize engine, takes the dimensions and count of discs
disc_engine :: disc_engine(float _x_limit, float _y_limit, int) : x_limit(_x_limit), y_limit(_y_limit)
{

}


//Advance state of system
disc_engine::state disc_engine :: advance(float t)
{
	disc_engine::state state = STATIC;
	vector<disc> :: size_type size = discs.size();
	for(unsigned int i = 0; i < size; i++)
	{
		if(discs[i].advance(t))
			state = DYNAMIC;
	}
	handle_collisions();

	/*/system("cls");

	float total_momentum = 0;
	//calculate total momentum and print it
	for(unsigned int i = 0; i < size; i++)
	{
		total_momentum += discs[i].velocity.mag()*discs[i].velocity.mag()*discs[i].mass;
	}

	cout << "Total momentum: " << total_momentum << endl;*/

	return state;
}


//Draw engine
void disc_engine :: draw()
{
	for(vector<disc>::iterator i = discs.begin(); i < discs.end(); i++)
	{
		i->draw();
	}
}


//function to set velocity of a disc
bool disc_engine :: set_velocity(unsigned int index, const vec2d& vec)
{
	if(index >= discs.size())
		return false;
	discs[index].velocity = vec;
	return true;
}


//rule to sink a disc
bool disc_engine :: sink()
{
	return false;
}


//Function to text if a disc can be added to the system or not
bool disc_engine::can_add(const disc& _disc){
	float x = abs(_disc.position.x)+ _disc.rad;
	float y = abs(_disc.position.y) + _disc.rad;
	
	if(x > x_limit || y > y_limit)
		return false;

	for(vector<disc> :: iterator i = discs.begin(); i < discs.end(); i++)
	{
		if(i->colliding_with(_disc))
			return false;
	}
	return true;
}



//Every time the disc is added
bool disc_engine :: add_disc(const disc& _disc)
{
	if(can_add(_disc)){
		discs.insert(discs.begin(), _disc);
		return true;
	}
	return false;
}


//function to remove disc
bool disc_engine :: remove_disc(unsigned int index)
{
	if(index >= discs.size())
		return false;
	discs.erase(discs.begin()+index);
	return true;
}