#include "NyancatScene6k.h"
#include "../../core/ResourceManager.h"
#include "../../core/ConfigManager.h"
#include "../../core/InputManager.h"
#include "../../Utils.h"
#include "../../library/nlohmann/json.hpp"
#include "../SuccessScene/SuccessScene.h" // Include to call setResults()

#include <fstream>
#include <string>

using json = nlohmann::json;

Nyancat6kScene::Nyancat6kScene(ResourceManager* rm, ConfigManager* cm, InputManager* im, std::function<void(const std::string&)> scene_changer)
    : RM(rm), CM(cm), IM(im), change_scene(scene_changer), music(nullptr), background(nullptr), game(nullptr), is_pause(false) {}

void Nyancat6kScene::read_configs() {
    std::ifstream file("scenes/NyancatScene6k/configs.json");
    json j;
    file >> j;

    background_key = j.value("background", "Nyancat6k_background");
    pause_key = j.value("pause_Img", "Nyancat6k_pause");
    dance_key = j.value("dance_gif", "dance");
    bgm_key = j.value("bgm", "menu_bgm");
    text_key = j.value("score_text", "caviar_dreams");
}

void Nyancat6kScene::init() {
    read_configs();

    game = new GameHandler("songs/nyan_cat_6k/chart.json", "songs/nyan_cat_6k/config.json", "6k", [this](const std::string &scene){
        change_scene(scene);
    }, music);

    background = RM->get_image(background_key);
    pause_Img = RM->get_image(pause_key);
    gif = RM->get_gif(dance_key);
    font = RM->get_font(text_key, 45);
    large_font = RM->get_font(text_key, 60);
    music = RM->play_sound(bgm_key, ALLEGRO_PLAYMODE_ONCE);
    is_pause = false;
}

bool Nyancat6kScene::update() {
    RM->update_sounds();

    if (game) {
        game->update();
        game->handle_input();
        if (game->game_ended()) {
            // Store the results before transitioning to SUCCESS
            SuccessScene::setResults(game->get_perfect(), game->get_good(), game->get_miss(), game->get_score());
            RM->toggle_sound(music);
            change_scene("SUCCESS");
        }
        if (game->lost()){
            RM->toggle_sound(music);
            change_scene("LOSE");
        }
    }
    return true; 
}

void Nyancat6kScene::draw() {
    if (!background) return;

    al_draw_bitmap(background, 0, 0, 0);

    // Draw the game notes and objects
    if (game) game->draw();

    // Draw pause image if paused
    if (is_pause) {
        al_draw_bitmap(pause_Img, 0, 0, 0);
    }

    // Draw GIF animation
    algif_draw_gif(gif, 0, 0, 0);

    // Draw score and counts
    al_draw_text(large_font, al_map_rgb(78, 41, 19), 1800, 202, ALLEGRO_ALIGN_RIGHT, std::to_string(game->get_score()).c_str());
    al_draw_text(font, al_map_rgb(49, 62, 79), 1750, 337, ALLEGRO_ALIGN_RIGHT, std::to_string(game->get_perfect()).c_str());
    al_draw_text(font, al_map_rgb(84, 130, 50), 1750, 451, ALLEGRO_ALIGN_RIGHT, std::to_string(game->get_good()).c_str());
    al_draw_text(font, al_map_rgb(193, 0, 0), 1750, 565, ALLEGRO_ALIGN_RIGHT, std::to_string(game->get_miss()).c_str());
}

void Nyancat6kScene::handle_input() {
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

    if (IM->was_key_pressed(ALLEGRO_KEY_Q)) {
        is_pause = !is_pause;
        if (game) game->toggle_pause();
        RM->toggle_sound(music);
    }

    // Debug keys for jumping scenes
    if (IM->was_key_pressed(ALLEGRO_KEY_Y)) {
        if (game) {
            SuccessScene::setResults(game->get_perfect(), game->get_good(), game->get_miss(), game->get_score());
        }
        RM->toggle_sound(music);
        change_scene("SUCCESS");
    }

    if (IM->was_key_pressed(ALLEGRO_KEY_N)) {
        RM->toggle_sound(music);
        change_scene("LOSE");
        return;
    }

    if(is_pause&&IM->was_key_pressed(ALLEGRO_KEY_R)){
        change_scene("NYANCAT6K");
        return;
    }

    if(is_pause&&IM->was_key_pressed(ALLEGRO_KEY_ESCAPE)){
        change_scene("SONG");
        return;
    }
}
