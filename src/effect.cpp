#include "effect.h"

// Constructors

Effect::Effect(const std::string &type, int duration, int potency, bool isNegative, int damagePerTurn) : type(type), duration(duration), maxDuration(duration), potency(potency), isNegative(isNegative), damagePerTurn(damagePerTurn)
{
}

// Getters

std::string Effect::getType() const
{
    return type;
}

int Effect::getDuration() const
{
    return duration;
}

int Effect::getMaxDuration() const
{
    return maxDuration;
}

int Effect::getPotency() const
{
    return potency;
}

bool Effect::getIsNegative() const
{
    return isNegative;
}

int Effect::getDamagePerTurn() const
{
    return damagePerTurn;
}

// Methods

void Effect::tick(){
    duration -= 1;
}

bool Effect::isExpired() const{
    if(duration == 0) return true;
    return false;
}

void Effect::reset(){
    duration = maxDuration;
}

void Effect::setDuration(int d){
    duration = d;
    maxDuration = d;
}