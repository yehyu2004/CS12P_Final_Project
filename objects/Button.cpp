#include "Button.h"


void Button::init() {
	IC = ImageCenter::get_instance();

	image = IC->get(image_path);
	width = al_get_bitmap_width(image);
	height = al_get_bitmap_height(image);
}

void Button::update(int mouse_x, int mouse_y) {
	if (mouse_x >= x && mouse_x <= x + 150 && mouse_y >= y && mouse_y <= y + 150) {
		mouse_touched = true;	
	} else {
		mouse_touched = false;
	}
}

void Button::clicked() {
	if(mouse_touched){
		if(!ran_action){
			ran_action = true;
			action();
		}
	}
}


void Button::draw() {
	if (mouse_touched) {
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