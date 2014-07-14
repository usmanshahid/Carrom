#pragma once
#include "player.h"
#include "board.h"

class game
{
	player players[4];

public:
	//constructor to initialize game(dimensions, rules)
	game(void);
	~game();
	
	void turn_change();

	//method to implement rule on board
	//method to enforce the game format
	//win/lose method
};