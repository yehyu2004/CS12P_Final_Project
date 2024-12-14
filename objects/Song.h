#ifndef SONG_H_INCLUDED
#define SONG_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <functional>

class Song{
private:
	int x, y, width, height;
	std::string image_path;
	bool character_touched, ran_action;
	ALLEGRO_BITMAP *image;
	std::function<void()> action;
public:
	void init();
	void draw();
	void update(int mouse_x, int mouse_y);
	void character_jumped();
	bool is_character_touched(){ return character_touched; }
	Song(int x_, int y_, ALLEGRO_BITMAP *image_){
		x = x_;
		y = y_;
		character_touched = false;
		ran_action = false;
		image = image_;
		init();
		action = [&](){

		};
	}

	Song(int x_, int y_, ALLEGRO_BITMAP *image_, std::function<void()> action_){
		x = x_;
		y = y_;
		character_touched = false;
		ran_action = false;
		image = image_;
		action = action_;
		init();
	}
};
#endif