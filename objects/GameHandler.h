#ifndef GAMEHANDLER_H_INCLUDED
#define GAMEHANDLER_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <functional>
#include <vector>
#include <array>
#include <deque>
#include <queue>
#include "../core/ChartLoader.h"
#include "Note.h"

const int PERFECT_SCORE = 500, GOOD_SCORE = 300, MISS_SCORE = 0;
// const double FALL_SPEED = 50, FALL_LENGTH = 925;

class GameHandler{
private:	
	InputManager *IM;
	ChartLoader *CL;
	std::queue<Note> notes;
	std::deque<Note> current_notes;
	std::string game_type;
	int perfect_count, good_count, miss_count, start_time;
	bool is_init;
public:
	void draw();
	void update();
	void handle_input();
	int get_perfect() { return perfect_count; }
	int get_good() { return good_count; }
	int get_miss() { return miss_count; }
	int get_score() { return perfect_count * PERFECT_SCORE + good_count * GOOD_SCORE + miss_count * MISS_SCORE; }
	GameHandler(std::string chart_path, std::string song_config_path, std::string game_type_);
};
#endif