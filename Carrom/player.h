#pragma once

#include <string>
#include "disc.h"
#include <vector>

struct player
{
	std::string name;
	int score;
	vector<disc> stack;
	int position;
};