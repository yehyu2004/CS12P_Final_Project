#include "NyancatScene4k.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

Nyancat4kScene::Nyancat4kScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void Nyancat4kScene::read_configs(){
    std::ifstream file("scenes/NyancatScene4k/configs.json");

    json j;
    file >> j;

    // Now read the keys
    background_key = j.value("background", "Nyancat4k_background");
    pause_key = j.value("pause_Img", "Nyancat4k_pause");
    key1_key = j.value("key1", "key4_1");
    key2_key = j.value("key2", "key4_2");
    key3_key = j.value("key3", "key4_3");
    key4_key = j.value("key4", "key4_4");
    dance_key = j.value("dance_gif", "dance");
    bgm_key = j.value("bgm", "menu_bgm");
    text_key = j.value("score_text", "caviar_dream");
}

void Nyancat4kScene::init() {
    read_configs();
    
    background = RM->get_image(background_key);
    pause_Img = RM->get_image(pause_key);
    key1 = RM->get_image(key1_key);
    key2 = RM->get_image(key2_key);
    key3 = RM->get_image(key3_key);
    key4 = RM->get_image(key4_key);
    gif = RM->get_gif(dance_key);
    font = RM->get_font(text_key,45);
    large_font = RM->get_font(text_key,60);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);
    is_pause=false;
}

bool Nyancat4kScene::update() {
    //if(is_pause) return;
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void Nyancat4kScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);   
    al_draw_bitmap(key1, 0, -200, 0);
    al_draw_bitmap(key2, 0, -200, 0);   
    al_draw_bitmap(key3, 0, -200, 0);
    al_draw_bitmap(key4, 0, -200, 0);
    
     
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

void Nyancat4kScene::handle_input() {
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
