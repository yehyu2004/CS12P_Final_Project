#include "GalleryScene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"

GalleryScene::GalleryScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
: RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void GalleryScene::init() {
    background = RM->get_image("gallery_background");

    // Play menu BGM
    music = RM->play_sound("menu_bgm", ALLEGRO_PLAYMODE_LOOP);
}

bool GalleryScene::update() {
    RM->update_sounds();
    return true; // return false if you want the game to end
}

void GalleryScene::draw() {
    if(!background) return;
    al_draw_bitmap(background, 0, 0, 0);    
    for (auto &btn : buttons) {
        btn.draw();
    }
    // Draw mouse cursor or other UI
}

void GalleryScene::handle_input() {
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

    if(IM->was_key_pressed(ALLEGRO_KEY_P)){
        RM->toggle_sound(music);
        change_scene("MENU");
    }
}