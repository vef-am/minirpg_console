#ifndef CHARACTER_H
#define CHARACTER_H

#ifndef NO_DIAGRAM
#include <iostream>
#include <string>
#endif

class Character {
    private:

    /** @brief Character's name */
    std::string name;

    /** @brief Character's actual health */
    int health;

    /** @brief Character's max health */
    int maxHp;

    /** @brief Character's armor */
    float armor;

};

#endif