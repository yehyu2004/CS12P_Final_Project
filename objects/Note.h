#ifndef NOTE_H_INCLUDED
#define NOTE_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include <array>
#include <vector>
#include "../core/InputManager.h"

const std::vector<std::pair<double,double>> init_coords_4k = {
    {400, 0}, {625, 0}, {850, 0}, {1075,0},
};

const std::vector<std::pair<double,double>> init_coords_6k = {
    {400,100}, {550,100}, {700,100},
    {850,100}, {1000,100}, {1150,100},
};

const std::vector<std::array<int,3>> key_note_color_4k = {
    {255,0,0}, {0,255,0}, {0,0,255}, {255,255,0}
};

const std::vector<std::array<int,3>> key_note_color_6k = {
    {255,0,0}, {0,255,0}, {0,0,255},
    {255,255,0}, {255,0,255}, {0,255,255}
};

const int NOTE_WIDTH = 150;
const int NOTE_HEIGHT = 100;
const double FALL_SPEED = 30;
const double FALL_LENGTH = 925;
const double FPS = 60; // Temporarily
const double PERFECT_FRAME = 80;
const double GOOD_FRAME = 120;
const double MISS_FRAME = 200;

enum SCORE_TYPE {
    PERFECT, GOOD, MISS, NONE
};

class Note {
private:
    double time;    // time in ms when the note should be hit
    double x, y;
    int col;
    std::string type;     // "tap" or "hold"
    std::string game_type;
    double duration;       // duration in ms (only for hold notes, 0 for tap)

    bool check_in_range(double left, double right);

public:
    Note(double time_, int col_, std::string type_, std::string game_type_, double duration_ = 0);

    void draw();
    void update();
    SCORE_TYPE check_score(int key);

    double get_time() const { return time; }
    double get_y() const { return y; }
    double get_col() const { return col; }
    double get_duration() const { return duration; }
    bool is_hold_note() const { return (type == "hold"); }
};

#endif
