#include "Musicstart2Scene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

Musicstart2Scene::Musicstart2Scene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void Musicstart2Scene::read_configs(){
    std::ifstream file("scenes/Musicstart2Scene/configs.json");

    json j;
    file >> j;

    // Now read the keys
    background_key = j.value("background", "Musicstart2_background");
    bgm_key = j.value("bgm", "menu_bgm");
    Musicstart2_key = j.value("gif", "Musicstart2");
}

void Musicstart2Scene::init() {
    read_configs();
    
    background = RM->get_image(background_key);
    gif=RM->get_gif(Musicstart2_key);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);
    Musicstart2_time=al_get_time();
}

bool Musicstart2Scene::update() {
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void Musicstart2Scene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);
    algif_draw_gif(gif, 0, 0, 0);    
    // for (auto &btn : buttons) {
    //     btn.draw();
    // }
    // Draw mouse cursor or other UI
}

void Musicstart2Scene::handle_input() {
    // int mouse_x = IM->get_mouse_x();
    // int mouse_y = IM->get_mouse_y();

    // // Update buttons hover state
    // for (auto &btn : buttons) {
    //     btn.update(mouse_x, mouse_y);
    // }

    // if (IM->was_mouse_pressed(1)) {
    //     for (auto &btn : buttons) {
    //         btn.clicked();
    //     }
    // }

    // if(IM->was_key_pressed(ALLEGRO_KEY_P)){
    //     RM->toggle_sound(music);
    //     change_scene("MENU");
    // }
    if(al_get_time()-Musicstart2_time>2.5)
    {
        RM->toggle_sound(music);
        change_scene("NYANCAT6K");
        
        
    }
}
