#pragma once

#include<vector>
#include "pot.h"
#include "disc.h"
#include "vec2d.h"


class carrom_rules
{
public:
	carrom_rules(void);
	
	int sink(vector<pot>, disc);
	bool can_striker_hit_the_discs(vector<disc>, vec2d);
	bool is_striker_touching_line(disc, vec2d, vec2d);
	vector<disc> is_this_sequence_of_sink_illegal(vector<disc>);

	virtual ~carrom_rules(void);
};

