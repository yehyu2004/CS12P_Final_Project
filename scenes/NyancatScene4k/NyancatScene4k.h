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
    ResourceManager* RM;
    ConfigManager* CM;
    InputManager* IM;
    std::function<void(const std::string&)> change_scene;

    ALLEGRO_BITMAP* background,*pause_Img;
    std::vector<Button> buttons;
    ALLEGRO_SAMPLE_INSTANCE* music;
    std::string background_key, bgm_key,pause_key,dance_key,text_key;
    bool is_pause;
    ALGIF_ANIMATION *gif;
    ALLEGRO_FONT *font,*large_font;
    void read_configs();
};

#endif
