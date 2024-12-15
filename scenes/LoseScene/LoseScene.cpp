#include "LoseScene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

LoseScene::LoseScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void LoseScene::read_configs(){
    std::ifstream file("scenes/LoseScene/configs.json");

    json j;
    file >> j;

    // Now read the keys
    background_key = j.value("background", "Lose_background");
    bgm_key = j.value("bgm", "nyan_cat_full");
}

void LoseScene::init() {
    read_configs();
    
    background = RM->get_image(background_key);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);
}

bool LoseScene::update() {
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void LoseScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);   
     
    for (auto &btn : buttons) {
        btn.draw();
    }
    // Draw mouse cursor or other UI
}

void LoseScene::handle_input() {
    int mouse_x = IM->get_mouse_x();
    int mouse_y = IM->get_mouse_y();

    //Update buttons hover state
    for (auto &btn : buttons) {
        btn.update(mouse_x, mouse_y);
    }

    if (IM->was_mouse_pressed(1)) {
        for (auto &btn : buttons) {
            btn.clicked();
        }
    }

    if (IM->was_mouse_pressed(1)){
        RM->toggle_sound(music);
        change_scene("SONG");
    }
}
