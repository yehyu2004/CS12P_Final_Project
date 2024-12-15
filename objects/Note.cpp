#include "Note.h"
#include <iostream>


Note::Note(double time_, int col_, std::string type_, std::string game_type_):
time(time_), col(col_), type(type_), game_type(game_type_) {
	if(game_type == "4k") { // find a better way to handle this
		x = init_coords_4k[col].first;
		y = init_coords_4k[col].second;
	} else if (game_type == "6k") {
		x = init_coords_6k[col].first;
		y = init_coords_6k[col].second;
	}
}

void Note::draw(){
	if(game_type == "4k") {
		auto [R,G,B] = key_note_color_4k[col];
		al_draw_filled_rectangle(x, y, x + NOTE_WIDTH, y + NOTE_HEIGHT, al_map_rgb(R, G, B));
	} else if (game_type == "6k"){
		auto [R,G,B] = key_note_color_6k[col];
		al_draw_filled_rectangle(x, y, x + NOTE_WIDTH, y + NOTE_HEIGHT, al_map_rgb(R, G, B));
	}
}

void Note::update(){
	y += FALL_SPEED;
}


bool Note::check_in_range(double left, double right){ // range is [left, right]
	return y >= left && y <= right;	
}

SCORE_TYPE Note::check_score(int key){
	if(key != col) return SCORE_TYPE::NONE;
	if(check_in_range(925 - PERFECT_FRAME * (1.0 / FPS) * FALL_SPEED, 925 + PERFECT_FRAME * (1.0 / FPS) * FALL_SPEED)){
		// PERFECT
		return SCORE_TYPE::PERFECT;
	} else if (check_in_range(925 - GOOD_FRAME * (1.0 / FPS) * FALL_SPEED, 925 + GOOD_FRAME * (1.0 / FPS) * FALL_SPEED)){
		// GOOD
		return SCORE_TYPE::GOOD;
	} else if (check_in_range(925 - MISS_FRAME * (1.0 / FPS) * FALL_SPEED, 925 + MISS_FRAME * (1.0 / FPS) * FALL_SPEED)){
		return SCORE_TYPE::MISS;
	} else {
		return SCORE_TYPE::NONE;
	}
}