/** @file Character.cpp
 * @brief Code of the Character class
 */

#include "Character.h"

Character::Character() : name(""), health(0), maxHp(0), armor(0)
{
}

Character::Character(const std::string &name, int health, int armor) : name(name), health(health), maxHp(health), armor(armor)
{
    if (maxHp < 0) maxHp = 0;
    if (health < 0) health = 0;
    else if (health > maxHp) 
}