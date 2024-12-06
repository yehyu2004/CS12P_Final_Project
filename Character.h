#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <string>
#include <map>
#include "Object.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Rectangle.h"

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
    private:
        CharacterState state=CharacterState::LEFT;
        double speed=5;
        std::map<CharacterState,std::string> gifPath;
        CharacterName name;
        DataCenter *DC;
        GIFCenter *GIFC;
        int level;
    public:
        void init();
        void update();
        void draw();
};
#endif