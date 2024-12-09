#ifndef MUSICSTARTSCENE_H_INCLUDED
#define MUSICSTARTSCENE_H_INCLUDED

#include "../Scene.h"
#include <functional>
#include <vector>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include "../../objects/Button.h"
#include "../../algif5/algif.h"

class ResourceManager;
class ConfigManager;
class InputManager;

class MusicstartScene : public Scene {
public:
    MusicstartScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer);
    void init() override;
    bool update() override;
    void draw() override;
    void handle_input() override;

private:
    ResourceManager* RM;
    ConfigManager* CM;
    InputManager* IM;
    std::function<void(const std::string&)> change_scene;

    ALLEGRO_BITMAP* background;
    std::vector<Button> buttons;
    ALLEGRO_SAMPLE_INSTANCE* music;
    std::string background_key, bgm_key,musicstart_key;
    double musicstart_time,current_time;
    ALGIF_ANIMATION *gif;
    void read_configs();
};

#endif
