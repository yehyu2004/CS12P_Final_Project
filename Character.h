#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
#include <map>
#include "Object.h"

enum class CharacterState{
    LEFT,
    RIGHT,
    JUMP,
    CHARACTERSTATE_MAX
};

enum class CharacterName{
    MAYMON,
    AURORA,
    NORA,
    CHARACTERNAME_MAX
};

class Character : public Object
{
    public:
        void init();
        void update();
        void draw();

    private:
        CharacterState state=CharacterState::LEFT;
        double speed=5;
        std::map<CharacterState,std::string> gifPath;
        CharacterName name;

};
#endif