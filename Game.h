#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "core/ResourceManager.h"
#include "core/ConfigManager.h"
#include "core/InputManager.h"
#include "Character.h"
#include "objects/Button.h"
#include "Utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <iostream>

constexpr char play_music_key[] = "play_music_btn";
constexpr char gallery_key[] = "gallery_btn";
constexpr char character_key[] = "character_btn";

class Game {
private:
    enum class STATE {
        START,
        LEVEL,
        CHARACTER,
        SONG,
        GALLERY,
        PAUSE,
        END
    };

    bool bgm_playing;
    STATE state;
    STATE pre_state;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *game_icon;
    ALLEGRO_BITMAP *background;
    ALLEGRO_SAMPLE_INSTANCE *music;

    ResourceManager *RM;
    ConfigManager *CM;
    InputManager *IM;

    std::vector<Button> current_buttons;

    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;

    // If you previously had DC->character, now we manage character here:
    Character character;

public:
    Game();
    ~Game();
    void execute();
    void game_init();
    bool game_update();
    void game_draw();
    void draw_background();
    void draw_states();
    void draw_buttons();
    void change_state(Game::STATE new_state);
    void change_music(const char* sound_key);
    void check_current_state();
};

#endif
