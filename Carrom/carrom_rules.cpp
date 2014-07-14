#include "carrom_rules.h"


carrom_rules::carrom_rules(void){
}


carrom_rules::~carrom_rules(void){
}

	
int carrom_rules::sink(vector<pot> pots, disc _disc)
{
	//char direction[6]; no need. if conditions work faster

	if(_disc.velocity.mag() < 10)
	{
		for(unsigned int i = 0 ; i < pots.size(); ++i)
		{
			if((_disc.position - pots[i].Pos()).mag() <= 1.3 * (pots[i].Rad() - _disc.rad))
				return i;
		}
	}
	
	return -1;
}

bool carrom_rules::can_striker_hit_the_discs(vector<disc> discs, vec2d circle)
{

	vector<disc>::iterator it = discs.begin();


	vec2d player_line = circle;
	player_line.rotate(3.14/4);
	
	for(; it != discs.end(); ++it)
	{
		if((it->position.proj(player_line) - it->position).mag() - it->rad)
			return false;
	}

	return true;
}

bool carrom_rules::is_striker_touching_line(disc striker, vec2d left_circle, vec2d right_circle)
{
	vec2d left_proj = striker.position.proj(left_circle);
	vec2d right_proj = striker.position.proj(right_circle);

	vec2d left_dist = left_proj - striker.position;
	vec2d right_dist = right_proj - striker.position;


	if(left_dist.mag() > striker.rad  && right_dist.mag() > striker.rad)
		return true;
	
	return false;
}


vector<disc> carrom_rules::is_this_sequence_of_sink_illegal(vector<disc> discs) // returns unsinked discs
{
	vector<disc>::iterator it = discs.begin();

	vector<disc> unpotted_discs;
	disc temp;

	for( it = discs.begin(); it != discs.end() ; ++it)
	{
		if (it->type=="queen")
		{
			disc queen = *it;
			++it;

			if(it != discs.end())
			{
				break;
			}
			else
			{
				unpotted_discs.push_back(*it);
				unpotted_discs.push_back(queen);
			}
		}

		temp = *it;
	}

	return unpotted_discs;
}