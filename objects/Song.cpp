#include "Song.h"

void Song::init() {
	width = al_get_bitmap_width(image);
	height = al_get_bitmap_height(image);
}

void Song::update(int character_x, int character_y) {
	if (character_x >= (x + x + width) / 2 - width/2 && character_x <= (x + x + width) / 2 + width/2 && character_y >= y && character_y <= y + height) {
		character_touched = true;	
	} else {
		character_touched = false;
	}
}

bool Song::character_jumped() {
	if(character_touched){
		if(!ran_action){
			ran_action = true;
			action();
			return true;
		}
	}
	return false;
}


void Song::draw() {
	if (character_touched) {
		al_draw_scaled_bitmap(
		    image,                // source bitmap
		    0, 0,                 // source region: start at top-left corner
		    width, height, // source region dimensions: entire image
		    x - 50, y - 50,             // destination coordinates on the display
		    width * 1.2, height * 1.2,   // scaled width and height
		    0                     // no flags
		);
	} else {
		al_draw_scaled_bitmap(
		    image,                // source bitmap
		    0, 0,                 // source region: start at top-left corner
		    width, height, // source region dimensions: entire image
		    x,y,             // destination coordinates on the display
		    width, height,   // scaled width and height
		    0                     // no flags
		);
	}
}