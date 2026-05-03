/** @file Character.cpp
 * @brief Code of the Character class
 */

#include "character.h"

// Constructors

Character::Character(const std::string &name, const Attributes &attributes, int currentHp, int maxHp, int armor, int currentSp, int maxSp) : name(name), currentHp(currentHp), maxHp(maxHp), armor(armor), currentSp(currentSp), maxSp(maxSp), attributes(attributes), defenseBoost(0)
{
    if (this->maxHp < 0)
        throw std::invalid_argument("Error! The maximum health must be a non-negative number.");
    else if (this->maxHp == 0)
        this->maxHp = this->currentHp;

    if (this->currentHp < 0)
        throw std::invalid_argument("Error! The current health must be a non-negative number.");
    else if (this->currentHp > this->maxHp)
        this->currentHp = this->maxHp;

    if (this->maxSp < 0)
        throw std::invalid_argument("Error! The maximum skill points must be a non-negative number.");
    else if (this->maxSp == 0)
        this->maxSp = this->currentSp;

    if (this->currentSp < 0)
        throw std::invalid_argument("Error! The current skill points must be a non-negative number.");
    else if (this->currentSp > this->maxSp)
        this->currentSp = this->maxSp;

    if (this->armor < 0)
        this->armor = 0;
}

// Getters

std::string Character::getName() const
{
    return name;
}

int Character::getCurrentHp() const
{
    return currentHp;
}

int Character::getMaxHp() const
{
    return maxHp;
}

int Character::getArmor() const
{
    return armor;
}

int Character::getCurrentSp() const
{
    return currentSp;
}

int Character::getMaxSp() const
{
    return maxSp;
}

int Character::getStrength() const
{
    return attributes.strength;
}

int Character::getAgility() const
{
    return attributes.agility;
}

int Character::getConstitution() const
{
    return attributes.constitution;
}

int Character::getIntelligence() const
{
    return attributes.intelligence;
}

int Character::getWisdom() const
{
    return attributes.wisdom;
}

int Character::getCharisma() const
{
    return attributes.charisma;
}

const std::vector<Effect>& Character::getEffects() const
{
    return effects;
}

bool Character::isAlive() const
{
    return currentHp > 0;
}

// Setters

void Character::setHealth(int health)
{
    if (health < 0)
        health = 0;
    else if (health > maxHp)
        health = maxHp;

    currentHp = health;
}

void Character::setArmor(int armor)
{
    this->armor = (armor < 0) ? 0 : armor;
}

// Modifiers

// Accesors

bool Character::isDead() const
{
    return currentHp <= 0;
}

// Destructors