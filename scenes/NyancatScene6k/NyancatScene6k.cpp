#include "NyancatScene6k.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

Nyancat6kScene::Nyancat6kScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void Nyancat6kScene::read_configs(){
    std::ifstream file("scenes/NyancatScene6k/configs.json");

    json j;
    file >> j;

    // Now read the keys
    background_key = j.value("background", "Nyancat6k_background");
    pause_key = j.value("pause_Img", "Nyancat6k_pause");
    dance_key = j.value("dance_gif", "dance");
    text_key = j.value("score_text", "caviar_dream");
    bgm_key = j.value("bgm", "menu_bgm");
}

void Nyancat6kScene::init() {
    read_configs();
    
    background = RM->get_image(background_key);
    pause_Img = RM->get_image(pause_key);
    gif = RM->get_gif(dance_key);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);
    font = RM->get_font(text_key,45);
    large_font = RM->get_font(text_key,60);
    is_pause=false;
}

bool Nyancat6kScene::update() {
    //if(is_pause) return;
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void Nyancat6kScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);   
     
    // for (auto &btn : buttons) {
    //     btn.draw();
    // }
    // Draw mouse cursor or other UI
   if(is_pause){
        al_draw_bitmap(pause_Img, 0, 0, 0);       
    }
    algif_draw_gif(gif, 0, 0, 0);
    al_draw_text(large_font,al_map_rgb(78,41,19),1800,202,ALLEGRO_ALIGN_RIGHT,"1000"); //score, change "1000" to variable 
    al_draw_text(font,al_map_rgb(49,62,79),1750,337,ALLEGRO_ALIGN_RIGHT,"0"); //perfect count
    al_draw_text(font,al_map_rgb(84,130,50),1750,451,ALLEGRO_ALIGN_RIGHT,"0"); //good count
    al_draw_text(font,al_map_rgb(193,0,0),1750,565,ALLEGRO_ALIGN_RIGHT,"0"); //miss count
}

void Nyancat6kScene::handle_input() {
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

    if(IM->was_key_pressed(ALLEGRO_KEY_Q)){
        is_pause=(is_pause ? false:true);
        RM->toggle_sound(music);
        // change_scene("MENU");
    }
    if(IM->was_key_pressed(ALLEGRO_KEY_Y)){
        RM->toggle_sound(music);
        change_scene("SUCCESS");
    }
    if(IM->was_key_pressed(ALLEGRO_KEY_N)){
        RM->toggle_sound(music);
        change_scene("LOSE");
    }
}
