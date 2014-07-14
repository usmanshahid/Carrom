#include "pot.h"

/* diskToSink class */

//constructor
discToSink::discToSink(){
	height = MAX_HEIGHT;
	vel = 0;
}


/*-----------------------*/

/* Pot Class */

// overloaded constructor
pot::pot(vec2d p, float r){
	pos = p;
	rad = r;
}

/*-----------------------*/

// getters

float pot::Rad(){
	return rad;
}

vec2d pot::Pos(){
	return pos;
}

/*-----------------------*/

void pot::add(disc disk){
	discToSink dts;
	dts.disk = disk;
	discs.push_back(dts);
}

void pot::sink(float dt){
	
	const float acc = -9.8;

	// return if the disc vector is empty
	if(discs.size() <= 0)
		return;

	//change height
	for(unsigned int i = 0; i < discs.size(); i++){
		if(0)
			discs[i].disk.advance(dt);
		else{
			discs[i].vel = discs[i].vel + acc * (dt/1000);
			discs[i].height +=  discs[i].vel * (dt/1000);
			if(discs[i].height <= MIN_HEIGHT)
				discs.erase(discs.begin()+i);
		}
	}
}


void pot::draw(){
	
	
		// transparency code
	
		//float transparency = discs[discs.size() -1].transparency - (discs[discs.size() -1].height/10); // 1.0 
		//glColor4f(0.0, 0.0, 1.0, transparency);
	
	//draw the disk
	for(unsigned int i = 0; i < discs.size(); i++)
	{
		//push current state
		glPushMatrix();
			//move along z-axis
			glTranslatef(0, 0, discs[i].height);
			discs[i].disk.draw();
		glPopMatrix();
	}
}