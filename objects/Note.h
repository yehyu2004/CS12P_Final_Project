#ifndef NOTE_H_INCLUDED
#define NOTE_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "../core/InputManager.h"
#include <string>
#include <functional>
#include <vector>
#include <array>

const std::vector<std::pair<double,double>> init_coords_4k = {
	{400,0},
	{625,0},
	{850,0},
	{1075,0},
};

const std::vector<std::pair<double,double>> init_coords_6k = {
	{100,100},
	{200,100},
	{300,100},
	{400,100},
	{500,100},
	{600,100},
};

const std::vector<std::array<int,3>> key_note_color_4k = {
	{255,0,0}, // change later (RGB)
	{0,255,0},
	{0,0,255},
	{255,255,0}
};


const std::vector<std::array<int,3>> key_note_color_6k = {
	{255,0,0},
	{0,255,0},
	{0,0,255},
	{255,255,0},
	{255,0,255},
	{0,255,255}
};

const int NOTE_WIDTH = 200, NOTE_HEIGHT = 100;
const double FALL_SPEED = 50, FALL_LENGTH = 925;
const double FPS = 60; //temporaily
const double PERFECT_FRAME = 20, GOOD_FRAME = 40, MISS_FRAME = 60;

enum SCORE_TYPE{
	PERFECT, GOOD, MISS, NONE
};

class Note{
private:
	double time;
	double x, y;
	int col;
	InputManager *IM;
	std::string type, game_type; // can be tap or hold
	bool check_in_range(double left, double right);
public:
	void draw();
	void update();
	SCORE_TYPE check_score(int key);
	double get_time() { return time; }
	double get_y() { return y; }
	Note(double time_, int col_, std::string type_, std::string game_type_);
};
#endif