#ifndef SUCCESSSCENE_H_INCLUDED
#define SUCCESSSCENE_H_INCLUDED

#include "../Scene.h"
#include <functional>
#include <vector>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include "../../objects/Button.h"

class ResourceManager;
class ConfigManager;
class InputManager;

class SuccessScene : public Scene {
public:
    SuccessScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer);

    void init() override;
    bool update() override;
    void draw() override;
    void handle_input() override;

    // Method to set the game results from outside
    static void setResults(int perfect, int good, int miss, int score);

private:
    void read_configs();

    ResourceManager* RM;
    ConfigManager* CM;
    InputManager* IM;
    std::function<void(const std::string&)> change_scene;

    ALLEGRO_BITMAP* background;
    std::vector<Button> buttons;
    ALLEGRO_SAMPLE_INSTANCE* music;
    ALLEGRO_FONT* font;

    std::string background_key, bgm_key;

    // Static variables to hold results
    static int perfect_count;
    static int good_count;
    static int miss_count;
    static int score;
};

#endif
