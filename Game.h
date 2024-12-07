// Game.h
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "core/ResourceManager.h"
#include "core/ConfigManager.h"
#include "core/InputManager.h"
#include "objects/Button.h"
#include "scenes/Scene.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <memory>
#include <functional>
#include <string>

class Game {
public:
    Game();
    ~Game();
    void execute();

private:
    ResourceManager *RM;
    ConfigManager *CM;
    InputManager *IM;

    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;

    ALLEGRO_EVENT event;
    bool running;

    std::unique_ptr<Scene> current_scene;

    void init_addons();
    void init_events();
    void allegro_setup();
    void game_init();

    bool game_update();
    void game_draw();

    void change_scene(const std::string& scene_name);
};

#endif
