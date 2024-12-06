#include "Character.h"
#include <cstdio>
#include <vector>
#include <iostream>

// Configuration for character levels and positions
namespace CharacterSetting {
    // Y-coordinates for each "level"
    static std::vector<int> y_coordinate = {
        200,
        540,
        880,
    };
}

void Character::init() {
    RM = ResourceManager::get_instance();
    CM = ConfigManager::get_instance();
    IM = InputManager::get_instance();

    // Associate each state with a gif key (defined in resources.json)
    gifKeys[CharacterState::LEFT] = "character_left";
    gifKeys[CharacterState::RIGHT] = "character_right";
    gifKeys[CharacterState::JUMP] = "character_jump";

    // Default state
    state = CharacterState::LEFT;

    // Start position
    level = 1; // Middle level
    x = 500;
    y = CharacterSetting::y_coordinate[level];

}

void Character::update() {
    bool at_left_border = (x <= 0);
    bool at_right_border = (x >= CM->get_window_width());
    bool at_top_level = (level == 0);
    bool at_lowest_level = (level == 2);

    if (IM->is_key_down(ALLEGRO_KEY_LEFT)) {
        if(!at_left_border) x -= speed;
        if(at_left_border && !at_top_level) {
            level--;
            y = CharacterSetting::y_coordinate[level];
            x = CM->get_window_width() - 1;
        }
        state = CharacterState::LEFT;
    }

    if (IM->is_key_down(ALLEGRO_KEY_RIGHT)) {
        if(!at_right_border) x += speed;
        if(at_right_border && !at_lowest_level) {
            level++;
            y = CharacterSetting::y_coordinate[level];
            x = 1;
        }
        state = CharacterState::RIGHT;
    }

    if (IM->was_key_pressed(ALLEGRO_KEY_UP)) {
        if(!at_top_level) {
            level--;
            y = CharacterSetting::y_coordinate[level];
        }
    }

    if (IM->was_key_pressed(ALLEGRO_KEY_DOWN)) {
        if(!at_lowest_level) {
            level++;
            y = CharacterSetting::y_coordinate[level];
        }
    }

    if (IM->was_key_pressed(ALLEGRO_KEY_SPACE)) {
        state = CharacterState::JUMP;
    }
}

void Character::draw() {
    ALGIF_ANIMATION *gif = RM->get_gif(gifKeys[state]);
    // Draw the GIF centered at (x,y)
    // algif_draw_gif is from algif5 library.
    algif_draw_gif(gif, 
                   x - gif->width / 2.0f, 
                   y - gif->height / 2.0f, 
                   0);
}
