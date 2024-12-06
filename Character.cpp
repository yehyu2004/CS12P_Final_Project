#include "Character.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Rectangle.h"
#include <stdio.h>
namespace CharacterSetting
{
    static constexpr char gif_root_path[50] = "./assets/gif/Character";
    static constexpr char gif_postfix[][10] = {
        "left",
        "right",
        "jump",
    };
}
void Character::init()
{
    for (size_t type = 0; type < static_cast<size_t>(CharacterState::CHARACTERSTATE_MAX); ++type)
    {
        char buffer[50];
        sprintf(
            buffer, "%s/character_%s.gif",
            CharacterSetting::gif_root_path,
            CharacterSetting::gif_postfix[static_cast<int>(type)]);
        gifPath[static_cast<CharacterState>(type)] = std::string{buffer};
    }
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    int startWidth = 500, startHeight = 500;
    // shape.reset(new Rectangle{DC->window_width / 2,
    //                           DC->window_height / 2,
    //                           DC->window_width / 2 + gif->width,
    //                           DC->window_height / 2 + gif->height});
    shape.reset(new Rectangle{startWidth,
                              startHeight,
                              startWidth+ gif->width,
                              startHeight + gif->height});
}
void Character::update()
{
    DataCenter *DC = DataCenter::get_instance();
    if (DC->key_state[ALLEGRO_KEY_LEFT])
    {
        shape->update_center_x(shape->center_x() - speed);
        state = CharacterState::LEFT;
    }
    else if (DC->key_state[ALLEGRO_KEY_RIGHT])
    {
        shape->update_center_x(shape->center_x() + speed);
        state = CharacterState::RIGHT;
    }
    else if (DC->key_state[ALLEGRO_KEY_SPACE])
    {
        // shape->update_center_x(shape->center_x() - speed);
        state = CharacterState::JUMP;
    }
    
}
void Character::draw()
{
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                   shape->center_x() - gif->width / 2,
                   shape->center_y() - gif->height / 2,
                   0);
}