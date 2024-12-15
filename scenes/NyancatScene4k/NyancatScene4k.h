#ifndef NYANCAT4KSCENE_H_INCLUDED
#define NYANCAT4KSCENE_H_INCLUDED

#include "../Scene.h"
#include <functional>
#include <vector>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include "../../algif5/algif.h"
#include "../../objects/Button.h"
#include "../../objects/GameHandler.h"

class ResourceManager;
class ConfigManager;
class InputManager;

class Nyancat4kScene : public Scene {
public:
    Nyancat4kScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer);
    void init() override;
    bool update() override;
    void draw() override;
    void handle_input() override;

private:
    void read_configs();

    ResourceManager* RM;
    ConfigManager* CM;
    InputManager* IM;
    GameHandler* game;
    std::function<void(const std::string&)> change_scene;

    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* pause_Img;
    ALLEGRO_BITMAP* key1;
    ALLEGRO_BITMAP* key2;
    ALLEGRO_BITMAP* key3;
    ALLEGRO_BITMAP* key4;
    ALGIF_ANIMATION* gif;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* large_font;
    ALLEGRO_SAMPLE_INSTANCE* music;

    std::vector<Button> buttons;
    bool is_pause;

    std::string background_key, bgm_key, pause_key, dance_key, text_key;
    std::string key1_key, key2_key, key3_key, key4_key;
};

#endif
