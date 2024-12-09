#include "SongScene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

#include <iostream>
SongScene::SongScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr), character(new Character()){}

void SongScene::read_configs(){
    std::ifstream file("scenes/SongScene/configs.json");

    json j;
    file >> j;

    // Now read the keys
    background_key = j.value("background", "character_background");
    bgm_key = j.value("bgm", "menu_bgm");
}

void SongScene::init() {
    read_configs();
    
    background = RM->get_image(background_key);
    character->init();

    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);

    //song button
    Button song1_btn(250, 50, RM->get_image("song1_btn"));
    Button song2_btn(775, 50, RM->get_image("song2_btn"));
    Button song3_btn(1350, 50, RM->get_image("song3_btn"));
    Button song4_btn(250, 385, RM->get_image("song4_btn"));
    Button song5_btn(775, 385, RM->get_image("song5_btn"));
    Button song6_btn(1350, 385, RM->get_image("song6_btn"));
    Button song7_btn(250, 730, RM->get_image("song7_btn"));
    Button song8_btn(775, 730, RM->get_image("song8_btn"));
    Button song9_btn(1350, 730, RM->get_image("song9_btn"));

    buttons.push_back(song1_btn);
    buttons.push_back(song2_btn);
    buttons.push_back(song3_btn);
    buttons.push_back(song4_btn);
    buttons.push_back(song5_btn);
    buttons.push_back(song6_btn);
    buttons.push_back(song7_btn);
    buttons.push_back(song8_btn);
    buttons.push_back(song9_btn);
}

bool SongScene::update() {
    RM->update_sounds();
    if(character->has_init) character->update();
    return true; // return false if you want the game to end
}

void SongScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);
    for (auto &btn : buttons) {
        btn.draw();
    }
    if(character->has_init) character->draw();
}

void SongScene::handle_input() {
    int mouse_x = IM->get_mouse_x();
    int mouse_y = IM->get_mouse_y();

    // Update buttons hover state
    // for (auto &btn : buttons) {
    //     btn.update(mouse_x, mouse_y);
    // }

    if (IM->was_mouse_pressed(1)) {
        for (auto &btn : buttons) {
            btn.clicked();
        }
    }

    if(IM->was_key_pressed(ALLEGRO_KEY_P)){
        RM->toggle_sound(music);
        change_scene("MENU");
    }

    if(IM->was_key_pressed(ALLEGRO_KEY_4)){
        RM->toggle_sound(music);
        change_scene("MUSICSTART");
    }
    if(IM->was_key_pressed(ALLEGRO_KEY_6)){
        RM->toggle_sound(music);
        change_scene("MUSICSTART2");
    }
}
