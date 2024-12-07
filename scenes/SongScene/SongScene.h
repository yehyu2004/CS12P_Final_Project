#ifndef SONGSCENE_H_INCLUDED
#define SONGSCENE_H_INCLUDED

#include "../Scene.h"
#include <functional>
#include <vector>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include "../../objects/Button.h"
#include "../../objects/Character.h"

class ResourceManager;
class ConfigManager;
class InputManager;

class SongScene : public Scene {
public:
    SongScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer);
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
    Character *character;
};

#endif
