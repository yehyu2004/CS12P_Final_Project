#include "SuccessScene.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

// Static member initialization
int SuccessScene::perfect_count = 0;
int SuccessScene::good_count = 0;
int SuccessScene::miss_count = 0;
int SuccessScene::score = 0;

SuccessScene::SuccessScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
    : RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr) {}

void SuccessScene::read_configs() {
    std::ifstream file("scenes/SuccessScene/configs.json");
    json j;
    file >> j;

    background_key = j.value("background", "Success_background");
    bgm_key = j.value("bgm", "nyan_cat_full");
}

void SuccessScene::init() {
    read_configs();
    background = RM->get_image(background_key);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_LOOP);

    // Load a font for drawing the results
    font = RM->get_font("caviar_dreams", 60); // Ensure this font key is available
}

bool SuccessScene::update() {
    RM->update_sounds();
    return true;
}

void SuccessScene::draw() {
    if (!background) return;

    al_draw_bitmap(background, 0, 0, 0);

    // Draw buttons
    for (auto &btn : buttons) {
        btn.draw();
    }

    // Display the results (perfect, good, miss, score)
    // Position these as per your UI design:
    al_draw_text(font, al_map_rgb(0, 0, 0), 960, 400, ALLEGRO_ALIGN_CENTRE, ("Perfect: " + std::to_string(perfect_count)).c_str());
    al_draw_text(font, al_map_rgb(0, 0, 0), 960, 480, ALLEGRO_ALIGN_CENTRE, ("Good: " + std::to_string(good_count)).c_str());
    al_draw_text(font, al_map_rgb(0, 0, 0), 960, 560, ALLEGRO_ALIGN_CENTRE, ("Miss: " + std::to_string(miss_count)).c_str());
    al_draw_text(font, al_map_rgb(0, 0, 0), 960, 640, ALLEGRO_ALIGN_CENTRE, ("Score: " + std::to_string(score)).c_str());
}

void SuccessScene::handle_input() {
    int mouse_x = IM->get_mouse_x();
    int mouse_y = IM->get_mouse_y();

    for (auto &btn : buttons) {
        btn.update(mouse_x, mouse_y);
    }

    if (IM->was_mouse_pressed(1)) {
        for (auto &btn : buttons) {
            btn.clicked();
        }
    }

    // Example: Clicking transitions back to SONG scene
    if (IM->was_mouse_pressed(1)) {
        RM->toggle_sound(music);
        change_scene("SONG");
    }
}

// Static method to set the results
void SuccessScene::setResults(int perfect, int good, int miss, int sc) {
    perfect_count = perfect;
    good_count = good;
    miss_count = miss;
    score = sc;
}
