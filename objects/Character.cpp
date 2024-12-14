#include "Character.h"
#include <cstdio>
#include <iostream>

// Configuration for character levels and positions
namespace CharacterSetting {
    static std::vector<int> y_coordinate = {
        200,
        540,
        880,
    };
}

void Character::init() {
    has_init = true;
    RM = ResourceManager::get_instance();
    CM = ConfigManager::get_instance();
    IM = InputManager::get_instance();

    // Associate each state with a gif key (defined in resources.json)
    gifKeys[CharacterState::LEFT] = "character_left";
    gifKeys[CharacterState::RIGHT] = "character_right";
    gifKeys[CharacterState::JUMP] = "character_jump";

    // Default state
    state = CharacterState::LEFT;

    // Start level and position
    level = 1;
    x = 500;
    y = CharacterSetting::y_coordinate[level];
}

void Character::update() {
    // Determine boundaries and levels
    bool at_left_border = (x <= 0);
    bool at_right_border = (x >= CM->get_window_width());

    // Check movement keys and delegate
    if (IM->is_key_down(ALLEGRO_KEY_LEFT)) {
        handle_walk_left(at_left_border);
    }
    if (IM->is_key_down(ALLEGRO_KEY_RIGHT)) {
        handle_walk_right(at_right_border);
    }

    if (IM->was_key_pressed(ALLEGRO_KEY_UP)) {
        handle_move_up();
    }

    if (IM->was_key_pressed(ALLEGRO_KEY_DOWN)) {
        handle_move_down();
    }

    // if (IM->was_key_pressed(ALLEGRO_KEY_SPACE)) {
    //     handle_jump();
    // }
}

bool Character::check_jump(std::vector<Song> &songs){
    bool has_song_touched = false;
    for(auto &song : songs){
        if(song.is_character_touched()){
            has_song_touched = true;
        }
    }
    if(has_song_touched)
        handle_jump();

    return has_song_touched;
}

void Character::draw() {
    ALGIF_ANIMATION *gif = current_gif();
    // Draw the GIF centered at (x, y)
    algif_draw_gif(gif, 
                   x - gif->width / 2.0f, 
                   y - gif->height / 2.0f, 
                   0);
}

void Character::get_bounding_box(float &out_x, float &out_y, float &out_w, float &out_h) const {
    ALGIF_ANIMATION *gif = RM->get_gif(gifKeys.at(state));
    float w = gif->width;
    float h = gif->height;
    // Character is drawn centered at (x,y)
    // so bounding box: top-left corner is (x - w/2, y - h/2)
    out_x = x - w / 2.0f;
    out_y = y - h / 2.0f;
    out_w = w;
    out_h = h;
}

// Private helper functions:

void Character::handle_walk_left(bool at_left_border) {
    if (!at_left_border) {
        x -= speed;
    }
    // If at left border and not top level, move up a level to the left
    if (at_left_border) {
        level = (level - 1 + 3) % 3;
        y = CharacterSetting::y_coordinate[level];
        x = CM->get_window_width() - 1;
    }
    state = CharacterState::LEFT;
}

void Character::handle_walk_right(bool at_right_border) {
    if (!at_right_border) {
        x += speed;
    }
    // If at right border and not lowest level, move down a level to the right
    if (at_right_border) {
        level = (level + 1) % 3;
        y = CharacterSetting::y_coordinate[level];
        x = 1;
    }
    state = CharacterState::RIGHT;
}

void Character::handle_move_up() {
    level = (level - 1 + 3) % 3;
    y = CharacterSetting::y_coordinate[level];
}

void Character::handle_move_down() {
    level = (level + 1) % 3;
    y = CharacterSetting::y_coordinate[level];
}

void Character::handle_jump() {
    // Just change state to jump for now
    state = CharacterState::JUMP;
}
