#include "board.h"
#include "cloud.h"
#include "point_carrom_rules.h"


//Contructor to initialize board
board::board(float _dim, carrom_rules rules_type) : disc_engine(_dim, _dim), dim(_dim), striker(10, 0.3, vec2d(0, -2.7), vec2d(0, 0), vec3d(0.5, 0.5, 0.5), 0.6)
{
	state = SETTING_STRIKER_POSITION;
	left_circle = get_mapped_coordinates(tex_left_circle, vec2d(tex_dim, tex_dim), vec2d(dim, dim));
	right_circle = get_mapped_coordinates(tex_right_circle, vec2d(tex_dim, tex_dim), vec2d(dim, dim));
	circle_rad = tex_rad/tex_dim * dim;
	face_direction = vec2d(1, 0);
	power = 0;

	switch(rules_type)
	{
	case POINT_CARROM:
		rules_set = new point_carrom_rules;
		break;
	}
	reset();

	create_pots();

}

void board::create_pots(){
	
	vec2d p1_tex(18, 18);
	vec2d p2_tex(480, 16);
	vec2d p3_tex(480, 480);
	vec2d p4_tex(18, 480);
	vec2d world_dim(dim, dim);
	float pot_radius = pot_tex_radius / tex_dim * dim * 2;

	vec2d pot1_center = get_mapped_coordinates(p1_tex, vec2d(tex_dim, tex_dim), world_dim);
	vec2d pot2_center = get_mapped_coordinates(p2_tex, vec2d(tex_dim, tex_dim), world_dim);
	vec2d pot3_center = get_mapped_coordinates(p3_tex, vec2d(tex_dim, tex_dim), world_dim);
	vec2d pot4_center = get_mapped_coordinates(p4_tex, vec2d(tex_dim, tex_dim), world_dim);
	
	pot p1(pot1_center, pot_radius);
	pot p2(pot2_center, pot_radius);
	pot p3(pot3_center, pot_radius);
	pot p4(pot4_center, pot_radius);

	pots.push_back(p1);
	pots.push_back(p2);
	pots.push_back(p3);
	pots.push_back(p4);
}

board::~board(void)
{
	delete rules_set;
}

//function to reset all carrom men
void board::reset()
{

	while(remove_disc(0));

	//Adding striker
	//striker = disc(15, 0.3, vec2d(0, -2.7), vec2d(0, 0), vec3d(0.5, 0.5, 0.5));
	add_disc(disc(5.5, 0.17, vec2d(0, 0), vec2d(0, 0), vec3d(1, 0, 0)));

	//Adding other carommen
	for(double angle = PI/12; angle <= 2*PI+0.3; angle+= PI/10)
	{
		disc temp(5.5, 0.17, vec2d(1.2*cos(angle), 1.2*sin(angle)), vec2d(0, 0), vec3d(0.8, 0.8, 0.8));
		disc temp1(5.5, 0.17, vec2d(0.6*cos(angle), 0.6*sin(angle)), vec2d(0, 0), vec3d(0.2, 0.2, 0.2));
		cout << add_disc(temp) << add_disc(temp1) << endl;
	}
}


//function to be called on mouse move
void board::mouse_move(int x, int y){
	if(state == SETTING_STRIKER_POSITION){
		striker.color = vec3d(0.5, 0.5, 0.5);
		striker.position = get_mapped_coordinates(vec2d(x, y), vec2d(pwidth, pheight), vec2d(right_circle.x - striker.rad, circle_rad));
		striker.position.y += left_circle.y + 0.05;
		if(!rules_set->is_striker_touching_line(striker, left_circle, right_circle) || !can_add(striker))
			striker.color = vec3d(1, 0, 0);
	}
	else if(state == SETTING_STRIKER_DIRECTION){
		vec2d cursor_pos = get_mapped_coordinates(vec2d(x, y), vec2d(pwidth, pheight), vec2d(dim, dim));
		bool collision = false;
		float min_distance = -1;
		//striker.velocity = (cursor_pos - striker.position).unit();
		for(unsigned int i = 1; i < discs.size(); i++)
		{
			//collision test
			vec2d rel_position_vec = discs[i].position - discs[0].position;
			vec2d dir_vec = cursor_pos - discs[0].position;
			vec2d proj = rel_position_vec.proj(dir_vec);
			vec2d perpendicular_vec = rel_position_vec - proj;
			
			//if true then colliding
			if(perpendicular_vec.mag() <= (discs[0].rad + discs[i].rad) && sign(proj.unit().x) == sign(dir_vec.unit().x)){
				guidelines __guidelines;
				__guidelines.contactpoint_dist = proj.mag() - sqrt(pow((discs[0].rad + discs[i].rad), 2) - pow(perpendicular_vec.mag(), 2));
				__guidelines.dir_vec = dir_vec.unit() * __guidelines.contactpoint_dist;
				__guidelines.contactpoint_vector = rel_position_vec - __guidelines.dir_vec;
				if(!collision){
					min_distance = __guidelines.dir_vec.mag();
				}
				if(__guidelines.dir_vec.mag() <= min_distance){
					min_distance = __guidelines.dir_vec.mag();
					_guidelines = __guidelines;
				}
				_guidelines._contact_type = guidelines::DISC;
				collision = true;
			}
		}

		if(!collision){
			
			vec2d dir_vec = cursor_pos - discs[0].position;
			vec2d unit = dir_vec.unit();
			/*vec2d wall_vector;
			if(abs(unit.x) > abs(unit.y)){
				if(sign(unit.x) == 1){
					wall_vector = vec2d(1, 0);
				} else {
					wall_vector = vec2d(-1, 0);
				}
			} else if(abs(unit.x) < abs(unit.y)){
				if(sign(unit.y) == 1){
					wall_vector = vec2d(0, 1);
				} else {
					wall_vector = vec2d(0, -1);
				}
			}

			vec2d pos_proj = discs[0].position.proj(wall_vector);
			cout << wall_vector.x << " " << wall_vector.y << endl;
			cout << unit.x << " " << unit.y << endl;
			float dist = (dim-pos_proj.mag()-discs[0].rad)/cos(unit.angle(wall_vector));
			_guidelines.dir_vec = unit * dist;*/
			vec2d disp(sign(unit.x) * (dim - discs[0].rad) - discs[0].position.x, sign(unit.y) * (dim - discs[0].rad)- discs[0].position.y);
			//cout << discs[0].position << " ";
			//cout << disp << endl;
			vec2d time(disp.x / unit.x, disp.y / unit.y);
			if(time.x > time.y){
				disp.x = time.y * unit.x;
				_guidelines.dir_vec = disp;
			}else if(time.x < time.y){
				disp.y = time.x * unit.y;
				_guidelines.dir_vec = disp;
			}else{
				_guidelines.dir_vec = disp;
			}
			_guidelines._contact_type = guidelines::WALL;
		}
	}
}

//Function to be called on movement while dragging the cursor
void board::mouse_drag(int x, int y){
	if(state == SETTING_STRIKER_POWER){
		vec2d cursor_pos = get_mapped_coordinates(vec2d(x, y), vec2d(pwidth, pheight), vec2d(dim, dim));
		vec2d dir_vec = cursor_pos - discs[0].position;
		double angle = dir_vec.angle(_guidelines.dir_vec);
		power = MAX_POWER + (angle - PI)/PI * MAX_POWER;
		if(power < 0.5)
			power = 0;
		//cout << power << endl;
	}
}

//Function to be called on mouse click/release
void board::mouse_click(int button, int state, int x, int y){

	/*if(state == ANIMATING){
		float x1, y1;
		float w = 4;
		float h = 4;
		x1 = ((float)x/(float)pwidth)*w*2 - w;
		y1 = h - ((float)y/(float)pheight)*h*2;
		cout << x1 << " " << y1 << endl;
		set_velocity(0, vec2d(vec2d(x1, y1), 15));
	}*/

	if(board::state == SETTING_STRIKER_POSITION && button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if(!(striker.color.x ==1 && striker.color.y == 0 && striker.color.z == 0)){
			disc temp = striker;
			temp.transparency = 1.0;
			add_disc(temp);
			board::state = SETTING_STRIKER_DIRECTION;
		}
	}else if(board::state == SETTING_STRIKER_DIRECTION && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		/*vec2d cursor_pos = get_mapped_coordinates(vec2d(x, y), vec2d(pwidth, pheight), vec2d(dim, dim));
		vec2d dir_vec = cursor_pos - discs[0].position;
		old_dir_vec = dir_vec;*/
		power = 0;
		board::state = SETTING_STRIKER_POWER;
	} else if(board::state == SETTING_STRIKER_POWER && button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if(power == 0)
			board::state = SETTING_STRIKER_DIRECTION;
		else{
			discs[0].velocity = vec2d(_guidelines.dir_vec.unit(), power);
			board::state = ANIMATING;
		}
	}
}



//function to draw board
void board::draw(){
	if(state == SETTING_STRIKER_DIRECTION || state == SETTING_STRIKER_POWER){
		cout << striker.position << endl;
		glPushMatrix();
		glTranslatef(0, 0, 0.03);
		glLineWidth(2);
		glColor3f(0.7, 0.7, 0.7);
		vec2d circle_pos = discs[0].position + _guidelines.dir_vec;
		if(_guidelines._contact_type == guidelines::DISC){
			vec2d temp = _guidelines.dir_vec - _guidelines.contactpoint_vector;
			vec2d striker_line_start = circle_pos + (temp.unit() * discs[0].rad);
			vec2d striker_line_end = striker_line_start + temp.unit() * 1;
			vec2d disc_line_start = circle_pos + _guidelines.contactpoint_vector.unit() * (_guidelines.contactpoint_vector.mag() + discs[1].rad);
			vec2d disc_line_end = disc_line_start + _guidelines.contactpoint_vector;
			glPushMatrix();
				glBegin(GL_LINES);
					glVertex2f(striker_line_start.x, striker_line_start.y);
					glVertex2f(striker_line_end.x, striker_line_end.y);

					glVertex2f(disc_line_start.x, disc_line_start.y);
					glVertex2f(disc_line_end.x, disc_line_end.y);
				glEnd();
			glPopMatrix();
		}
		glPushMatrix();
			glBegin(GL_LINES);
				/*if(state == SETTING_STRIKER_POWER){
					vec2d separation_point = discs[0].position + _guidelines.dir_vec.unit() * power/MAX_POWER * _guidelines.dir_vec.mag();
						glColor3f(0, 1, 0);
						glVertex2f(discs[0].position.x, discs[0].position.y);
						glVertex2f(separation_point.x, separation_point.y);
						glColor3f(1, 0, 0);
						glVertex2f(separation_point.x, separation_point.y);
						glVertex2f(circle_pos.x, circle_pos.y);
				} else {*/
						glVertex2f(discs[0].position.x, discs[0].position.y);
						glVertex2f(circle_pos.x, circle_pos.y);
				//}
			glEnd();
			glTranslatef(circle_pos.x, circle_pos.y, 0);
			circle(discs[0].rad);
		glPopMatrix();
		glPopMatrix();
		if(state == SETTING_STRIKER_POWER){
			glPushMatrix();
				glTranslatef(discs[0].position.x, discs[0].position.y, 0.09);
				glColor4f(1, 0, 0, 0.9);
				glBindTexture(GL_TEXTURE_2D, disc_texture);
				filled_circle(discs[0].rad, _guidelines.dir_vec.angle(vec2d(1, 0)), _guidelines.dir_vec.angle(vec2d(1, 0)) + power/MAX_POWER * 2 * PI);
			glPopMatrix();
		}

		glLineWidth(1);
	}

	//drawing board
	glBindTexture(GL_TEXTURE_2D, board_texture);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex2f(-dim, -dim);
		glTexCoord2f(1, 0);
		glVertex2f(dim, -dim);
		glTexCoord2f(1, 1);
		glVertex2f(dim, dim);
		glTexCoord2f(0, 1);
		glVertex2f(-dim, dim);
	glEnd();

	//draw striker in setting state
	if(state == SETTING_STRIKER_POSITION)
		striker.draw();
	
	glColor3f(0.5, 1, 0.5);

	for(unsigned int i = 0; i < pots.size(); i++)
		pots[i].draw();
	disc_engine::draw();
}

//function to advance animation by delta t(in ms)
void board::advance(float dt){
	if(state == ANIMATING){
		disc_engine::state engine_state= disc_engine::advance(dt);
		if(engine_state == disc_engine::STATIC){
			remove_disc(0);
			state = SETTING_STRIKER_POSITION;
		}
	}

	for(unsigned int i = 0; i < discs.size(); i++){
		int pot_num = rules_set->sink(pots, discs[i]);
		if(pot_num != -1){
			pots[pot_num].add(discs[i]);
			remove_disc(i);
		}
	}

	// check the pots for gotees to sink
	for(int i = 0; i < pots.size(); i++)
		pots[i].sink(dt);
}

void board::add_to_pot(disc toSink, int potNumber){
	pots[potNumber].add(toSink);
}