#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include "../core/ConfigManager.h"
#include "../core/ResourceManager.h"
#include "../core/InputManager.h"
#include "../algif5/algif.h"
#include "Song.h"

enum class CharacterState {
    LEFT,
    RIGHT,
    JUMP,
    CHARACTERSTATE_MAX
};

enum class CharacterName {
    MAYMON,
    AURORA,
    NORA,
    CHARACTERNAME_MAX
};

class Character{
private:
    CharacterState state;
    float speed;
    std::map<CharacterState, std::string> gifKeys;
    CharacterName name;
    ResourceManager *RM;
    ConfigManager *CM;
    InputManager *IM;

    float x, y;
    int level;

    // Private helper functions for movement
    void handle_walk_left(bool at_left_border);
    void handle_walk_right(bool at_right_border);
    void handle_move_up();
    void handle_move_down();
    void handle_jump();

    // Helper for current gif
    ALGIF_ANIMATION* current_gif() const {
        return RM->get_gif(gifKeys.at(state));
    }
public:
    bool has_init;

    void init();
    void update();
    void draw();
    float get_x(){ return x; }
    float get_y(){ return y; }
    bool check_jump(std::vector<Song> &songs);
    Character() : state(CharacterState::LEFT), speed(5.0f), x(500.0f), y(0.0f), level(1), has_init(false) {}

    // Optional: For collision checks with other objects
    void get_bounding_box(float &out_x, float &out_y, float &out_w, float &out_h) const;
};

#endif
