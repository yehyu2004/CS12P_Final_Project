#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
#include <map>
#include "Object.h"
#include "core/ConfigManager.h"
#include "core/ResourceManager.h"
#include "core/InputManager.h"
#include "algif5/algif.h"

// States representing the character's direction/movement.
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

class Character : public Object {
private:
    CharacterState state = CharacterState::LEFT;
    double speed = 5.0;
    std::map<CharacterState, std::string> gifKeys;
    CharacterName name;
    ResourceManager *RM;
    ConfigManager *CM;
    InputManager *IM;

    // Instead of a shape, store character position and level directly.
    float x, y;
    int level;

public:
    void init();
    void update();
    void draw();
};

#endif
