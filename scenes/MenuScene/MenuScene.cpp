// MenuScene.cpp
#include "MenuScene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"

MenuScene::MenuScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), bgm_playing(false), music(nullptr), background(nullptr) {}

void MenuScene::init() {
    background = RM->get_image("menu_background");

    // Example buttons: 
    // Each button when clicked will call change_scene with the desired scene name
    Button play_music_btn(1300, 500, RM->get_image("play_music_btn"), [&](){ if(music) RM->toggle_sound(music); change_scene("SONG");});
    Button gallery_btn(1300, 700, RM->get_image("gallery_btn"), [&](){ if(music) RM->toggle_sound(music); change_scene("GALLERY");});
    Button character_btn(1300, 900, RM->get_image("character_btn"), [&](){ if(music) RM->toggle_sound(music); change_scene("CHARACTER");});

    buttons.push_back(play_music_btn);
    buttons.push_back(gallery_btn);
    buttons.push_back(character_btn);

    // Play menu BGM
    music = RM->play_sound("menu_bgm", ALLEGRO_PLAYMODE_LOOP);
    bgm_playing = true;
}

bool MenuScene::update() {
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void MenuScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);
    for (auto &btn : buttons) {
        btn.draw();
    }
    // Draw mouse cursor or other UI
}

void MenuScene::handle_input() {
    int mouse_x = IM->get_mouse_x();
    int mouse_y = IM->get_mouse_y();

    // Update buttons hover state
    for (auto &btn : buttons) {
        btn.update(mouse_x, mouse_y);
    }

    if (IM->was_mouse_pressed(1)) {
        for (auto &btn : buttons) {
            btn.clicked();
        }
    }
}
