#include <cmath>
#include "vec2d.h"


//--------Constructors
//Contructor for vector class given (x, y)(null vector by default)
vec2d :: vec2d(float _x, float _y)
{
	x=_x;
	y=_y;
}

//Constructor to initialize a vector with a vector and magnitude (the new vector will share the direction of given vector)
vec2d :: vec2d(vec2d vec, float mag)
{
	vec.normalize();
	if(mag == 0)
		throw "invalid vector";
	x=mag*vec.x;
	y=mag*vec.y;
}


//---------Basic arithmetic operations
//addition
vec2d vec2d :: operator+(const vec2d& vec) const
{
	return vec2d(x+vec.x, y+vec.y);
}

//Subtraction
vec2d vec2d :: operator-(const vec2d& vec) const
{
	return vec2d(x-vec.x, y-vec.y);
}

//--------Scalar operations
//Multiplication
vec2d vec2d :: operator*(float val) const
{
	return vec2d(x * val, y * val);
}

//Scalar division
vec2d vec2d :: operator/(float val) const
{
	return vec2d(x / val, y / val);
}


//--------Vector Product
//Dot product
float vec2d :: operator*(const vec2d& vec) const
{
	return x * vec.x + y * vec.y;
}

//Cross product
vec3d vec2d :: cross(const vec2d& vec) const
{
	vec3d rhs(x, y, 0);
	vec3d lhs(vec.x, vec.y, 0);

	return rhs.cross(lhs);
}

//--------Other operations
//to return magnitude of vector
float vec2d :: mag() const
{
	return sqrt(x*x + y*y);
}

//to return unit vector in the direction of vector
vec2d vec2d :: unit() const
{
	float m = mag();
	if(m == 0)
		throw "invalid vector";
	return vec2d(x/m, y/m);
}

//to normalize the vector
void vec2d :: normalize()
{
	float m = mag();
	if(m == 0)
		throw "invalid vector";
	x = x/m;
	y = y/m;
}

//to return the inverted vector
vec2d vec2d :: inverse() const
{
	return (*this)*-1;
}

//to return projection vector
vec2d vec2d :: proj(const vec2d& vec) const
{
	float m = vec.mag();
	if(m == 0)
		throw "invalid vector";
	return vec.unit()*(operator*(vec)/m);
}

//to return the angle between vectors in radians
double vec2d :: angle(const vec2d& vec) const
{
	float this_mag = mag();
	float vec_mag = vec.mag();
	if(this_mag == 0 || vec_mag == 0)
		throw "invalid vector";
	return acos(operator*(vec)/(this_mag*vec_mag));
}

//to rotate a vector by angle radians
void vec2d :: rotate(double angle)
{
	float m = mag();
	if(m==0)
		throw "invalid vector";
	double a = this->angle(vec2d(1, 0));
	a += angle;
	x = m * (float)cos(a);
	y = m * (float)sin(a);
}

//to compare two vectors
bool vec2d::operator==(const vec2d& vec){
	return x == vec.x && y == vec.y;
}

//friend stream insertion and extraction
ostream& operator<<(ostream& out, const vec2d& vec)
{
	out << "[" << vec.x << ", " << vec.y << "]";
	return out;
}