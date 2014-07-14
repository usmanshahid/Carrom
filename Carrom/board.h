#pragma once

#include <vector>
#include "disc_engine.h"
#include "carrom_rules.h"
#include "gl/glut.h"
#include "SOIL.h"
#include "disc.h"
#include "pot.h"

enum board_state;

class board : protected disc_engine
{
	float dim;
	carrom_rules * rules_set;
	vector<pot> pots;
	static const int MAX_POWER = 20;

	//function to reset all carrom men positions
	void reset();

	//create pots
	void create_pots();

	//State variables
	vector<disc> potted_discs;
	disc striker;
	//vec2d old_dir_vec;
	float power;
	enum board_state{ANIMATING, SETTING_STRIKER_POSITION, SETTING_STRIKER_DIRECTION, SETTING_STRIKER_POWER};
	board_state state;
	vec2d face_direction;
	vec2d right_circle;
	vec2d left_circle;
	float circle_rad;

	struct guidelines{
		enum contact_type{WALL, DISC} _contact_type;
		vec2d dir_vec;
		float contactpoint_dist;
		vec2d contactpoint_vector;
	}_guidelines;
	
public:
	enum carrom_rules {POINT_CARROM};

	//Constructor to set board
	board(float, carrom_rules);
	
	//function to advance animation by delta t(in ms)
	void advance(float);

	//function to be called on mouse move
	void mouse_move(int, int);

	//Function to be called on movement while dragging the cursor
	void mouse_drag(int, int);

	//Function to be called on mouse click/release
	void mouse_click(int, int, int, int);

	//function to draw board
	void draw();

	//function to sink the goti
	void add_to_pot(disc toSink, int potNumber);

	//Default destructor
	~board(void);
};

