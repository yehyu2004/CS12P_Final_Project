#include "CharacterScene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

CharacterScene::CharacterScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void CharacterScene::read_configs(){
    std::ifstream file("scenes/CharacterScene/configs.json");

    json j;
    file >> j;

    // Now read the keys
    background_key = j.value("background", "character_background");
    bgm_key = j.value("bgm", "menu_bgm");
    au_key = j.value("au_gif", "aurora_selected");
    ma_key = j.value("ma_gif", "maymon_selected");
    na_key = j.value("na_gif", "nora_selected");
}

void CharacterScene::init() {
    read_configs();

    background = RM->get_image(background_key);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);
    is_selected=false;
}

bool CharacterScene::update() {
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void CharacterScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);
    // for (auto &btn : buttons) {
    //     btn.draw();
    // }
    if(is_selected)
    {
        algif_draw_gif(gif, 0, 0, 0);
    }
}

void CharacterScene::handle_input() {
    int mouse_x = IM->get_mouse_x();
    int mouse_y = IM->get_mouse_y();

    // Update buttons hover state
    // for (auto &btn : buttons) {
    //     btn.update(mouse_x, mouse_y);
    // }

    if (IM->was_mouse_pressed(1)) {
        // for (auto &btn : buttons) {
        //     btn.clicked();
        // }
        int x=IM->get_mouse_x();
        int y=IM->get_mouse_y();
        if(x>=0 && x<=896 && !is_selected){
            is_selected=true;
            gif=RM->get_gif(ma_key);
    
        }else if(x>=898 && x<=1472 && !is_selected){
            is_selected=true;
            gif=RM->get_gif(au_key);

        }else if(x>=1475 && x<=1920 && !is_selected){
            is_selected=true;
            gif=RM->get_gif(na_key);
        }else if(is_selected)
        {
            is_selected=false;
        }
    }

    if(IM->was_key_pressed(ALLEGRO_KEY_P)){
        RM->toggle_sound(music);
        change_scene("MENU");
    }
}
