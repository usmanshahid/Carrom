#pragma once
#include <iostream>
#include "vec3d.h"

using namespace std;


//class for 2 dimentional vectors
//make sure u add checks for null vector and other corner cases throw (char*) exceptions if required

class vec2d
{
public:
	//public x,y components
	float x, y;

	//--------Constructors
	//Contructor for vector class given (x, y)(null vector by default)
	vec2d(float = 0, float = 0);

	//Constructor to initialize a vector with a vector and magnitude (the new vector will share the direction of given vector)
	vec2d(vec2d, float);


	//---------Basic arithmetic operations
	//addition
	vec2d operator+(const vec2d&) const;

	//Subtraction
	vec2d operator-(const vec2d&) const;


	//--------Scalar operations
	//Multiplication
	vec2d operator*(float) const;

	//Scalar division
	vec2d operator/(float) const;


	//--------Vector Product
	//Dot product
	float operator*(const vec2d&) const;

	//Cross product
	vec3d cross(const vec2d&) const;


	//--------Other operations
	//to return magnitude of vector
	float mag() const;

	//to return unit vector in the direction of vector
	vec2d unit() const;

	//to normalize the vector
	void normalize();

	//to return the inverted vector
	vec2d inverse() const;

	//to return projection vector
	vec2d proj(const vec2d&) const;

	//to return the angle between vectors in radians
	double angle(const vec2d&) const;

	//to rotate a vector by angle radians
	void rotate(double);

	//to compare two vectors
	bool operator==(const vec2d&);

	//friend stream insertion and extraction
	friend ostream& operator<<(ostream&, const vec2d&);
	//friend istream& operator>>(istream&, vec2d&);
};

