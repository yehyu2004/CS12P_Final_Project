#ifndef GAMEHANDLER_H_INCLUDED
#define GAMEHANDLER_H_INCLUDED

#include <allegro5/allegro.h>
#include <string>
#include <deque>
#include <queue>
#include <functional>
#include "../core/ChartLoader.h"
#include "../core/InputManager.h"
#include "../core/ResourceManager.h"
#include "Note.h"

const int PERFECT_SCORE = 500;
const int GOOD_SCORE = 300;
const int MISS_SCORE = 0;
const int SCORE_TIME = 500;

const int HEALTH_MAX = 100;
const int HEALTH_LOSS_MISS = 10; 
const int HEALTH_GAIN_GOOD = 3;  
const int HEALTH_GAIN_PERFECT = 5;

class GameHandler {
private:
    InputManager *IM;
    ResourceManager *RM;
    ChartLoader *CL;

    std::queue<Note> notes;
    std::deque<Note> current_notes;
    std::deque<std::pair<double, std::pair<SCORE_TYPE, int>>> current_scores;

    std::string game_type;
    ALLEGRO_FONT *font;
    double perfect_count;
    double good_count;
    double miss_count;
    double start_time;
    double song_duration;
    double pause_time;
    double recover_time;
    bool is_init;
    bool is_paused;

    int health;

    std::function<void(const std::string&)> scene_changer;
    ALLEGRO_SAMPLE_INSTANCE* music; // We'll store this to toggle sound on loss

public:
    GameHandler(std::string chart_path, std::string song_config_path, std::string game_type_,
                std::function<void(const std::string&)> scene_changer_, ALLEGRO_SAMPLE_INSTANCE* music_);

    void draw();
    void update();
    void handle_input();
    void toggle_pause();

    int get_perfect() { return static_cast<int>(perfect_count); }
    int get_good() { return static_cast<int>(good_count); }
    int get_miss() { return static_cast<int>(miss_count); }
    int get_score() { return static_cast<int>(perfect_count * PERFECT_SCORE + good_count * GOOD_SCORE + miss_count * MISS_SCORE); }

    bool game_ended();

    void adjust_health(int amount) {
        health += amount;
        if (health > HEALTH_MAX) health = HEALTH_MAX;
        if (health < 0) health = 0;
    }

    int get_health() const { return health; }

    bool lost() const { return health <= 0; }
};

#endif
