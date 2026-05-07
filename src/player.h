#ifndef PLAYER_H
#define PLAYER_H

#ifndef NO_DIAGRAM
#include <string>
#endif

#include "character.h"

class Player : public Character
{
public:
    int Attack();
};

#endif
