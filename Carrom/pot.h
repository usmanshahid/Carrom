#pragma once
#include "disc_engine.h"
#include "gl/glut.h"
#include "SOIL.h"

#define MAX_HEIGHT 0
#define MIN_HEIGHT -1

class discToSink{
public:	
	disc disk;
	float height;
	float vel;
	discToSink();
};

class pot
{
	float rad;
	vec2d pos;
	vector<discToSink> discs;

public:
	pot(vec2d, float);
	
	// getters:
	float Rad();
	vec2d Pos();

	// add disc to sink
	void add(disc);

	//sink funtion
	void sink(float);

	//draw function
	void draw();
};

