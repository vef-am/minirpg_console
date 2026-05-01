/** @file Character.cpp
 * @brief Code of the Character class
 */

#include "character.h"

// Constructors

Character::Character() : name(""), health(-1), maxHp(-1), armor(0)
{
}

Character::Character(const std::string &name, int health, int armor) : name(name), health(health), maxHp(health), armor(armor)
{
    if (maxHp < 0)
        maxHp = 0;

    if (health < 0)
        health = 0;
    else if (health > maxHp)
        health = maxHp;

    if (armor < 0)
        armor = 0;
}

// Getters

std::string Character::getName() const
{
    return this->name;
}

int Character::getHealth() const
{
    return this->health;
}

int Character::getMaxHp() const
{
    return this->maxHp;
}

int Character::getArmor() const
{
    return this->armor;
}

// Setters

int Character::setHealth(int health)
{
    if(health < 0) health = 0;
    else if(health > maxHp) health = maxHp;

    this->health = health;
}

int Character::setArmor(int armor)
{
}

// Modifiers

// Accesors

bool Character::isDead() const
{
    if (this->health == 0)
        return true;
    return false;
}

// Destructors