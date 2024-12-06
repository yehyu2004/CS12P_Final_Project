#include "Character.h"
#include <stdio.h>
#include <vector>

#include <iostream>

namespace CharacterSetting
{
    static constexpr char gif_root_path[50] = "./assets/gif/Character";
    static constexpr char gif_postfix[][10] = {
        "left",
        "right",
        "jump",
    };
    static std::vector<int> y_coordinate = {
        200,
        540,
        880,
    };
}
void Character::init()
{
    DC = DataCenter::get_instance();
    GIFC = GIFCenter::get_instance();
    for (size_t type = 0; type < static_cast<size_t>(CharacterState::CHARACTERSTATE_MAX); ++type)
    {
        char buffer[50];
        sprintf(
            buffer, "%s/character_%s.gif",
            CharacterSetting::gif_root_path,
            CharacterSetting::gif_postfix[static_cast<int>(type)]);
        gifPath[static_cast<CharacterState>(type)] = std::string{buffer};
    }
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    level = 1;
    int startX = 500, startY = 500;

    shape.reset(new Rectangle{startX,
                              startY,
                              startX + gif->width,
                              startY + gif->height});
    shape->update_center_y(CharacterSetting::y_coordinate[level]);

    // std::cout << "CENTER!  " << shape->center_y() << std::endl;
}
void Character::update()
{
    bool at_left_border = (shape->center_x() <= 0);
    bool at_right_border = (shape->center_x() >= DC->window_width);
    bool at_top_level = (level == 0);
    bool at_lowest_level = (level == 2);
    // std::cout << "CENTER!  " << shape->center_y() << std::endl;
    // std::cout << "CURRENT LEVEL  " << level << std::endl;
    if (DC->key_state[ALLEGRO_KEY_LEFT]){   
        if(!at_left_border){
            shape->update_center_x(shape->center_x() - speed);
        }
        if(at_left_border && !at_top_level){
            level--;
            shape->update_center_y(CharacterSetting::y_coordinate[level]);
            shape->update_center_x(DC->window_width - 1);
        }
        state = CharacterState::LEFT;
    }

    if (DC->key_state[ALLEGRO_KEY_RIGHT]){
        if(!at_right_border){
            shape->update_center_x(shape->center_x() + speed);
        }
        if(at_right_border && !at_lowest_level){
            level++;
            shape->update_center_y(CharacterSetting::y_coordinate[level]);
            shape->update_center_x(1);
        }
        state = CharacterState::RIGHT;
    }
    if (DC->key_state[ALLEGRO_KEY_UP]){   
        if(!at_top_level){
            level--;
            shape->update_center_y(CharacterSetting::y_coordinate[level]);
        }
    }
    if (DC->key_state[ALLEGRO_KEY_DOWN]){
        if(!at_lowest_level){
            level++;
            shape->update_center_y(CharacterSetting::y_coordinate[level]);
        }
    }
    if (DC->key_state[ALLEGRO_KEY_SPACE]){
        // shape->update_center_x(shape->center_x() - speed);
        state = CharacterState::JUMP;
    }
    
}
void Character::draw()
{
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                   shape->center_x() - gif->width / 2,
                   shape->center_y() - gif->height / 2,
                   0);
}