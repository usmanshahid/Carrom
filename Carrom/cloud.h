#pragma once

#include "vec2d.h"

//Function to calculate distance in 2d plane
double distance2d(const vec2d&, const vec2d&);
double distance3d(const vec3d&, const vec3d&);


//Texture
extern GLuint board_texture, disc_texture, disc_boundary_texture;
extern float pwidth, pheight;

void load_textures();

//Function to convert texture/Screen coordinates in board coordinates
vec2d get_mapped_coordinates(const vec2d&, const vec2d&, const vec2d&);

//function to return sign
int sign(double);


//Shapes drawings
void circle(float);
void filled_circle(float, float, float);
void disk(float);
void open_cylinder(vec3d, vec3d, float, float);
