#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "../data/ImageCenter.h"
#include <string>
#include <functional>

class Button{
private:
	ImageCenter *IC;
	int x, y, width, height;
	std::string image_path;
	bool mouse_touched, ran_action;
	ALLEGRO_BITMAP *image;
	std::function<void()> action;
public:
	void init();
	void draw();
	void update(int mouse_x, int mouse_y);
	void clicked();
	Button(int x_, int y_, std::string image_path_){
		x = x_;
		y = y_;
		mouse_touched = false;
		ran_action = false;
		image = nullptr;
		image_path = image_path_;
		init();
		action = [&](){

		};
	}

	Button(int x_, int y_, std::string image_path_, std::function<void()> action_){
		x = x_;
		y = y_;
		mouse_touched = false;
		ran_action = false;
		image = nullptr;
		image_path = image_path_;
		action = action_;
		init();
	}
};
#endif