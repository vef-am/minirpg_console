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

const std::vector<Effect> &Character::getEffects() const
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

void Character::addSp(int skillPoints)
{
    if (skillPoints > 0)
        currentSp = (currentSp + skillPoints > maxSp) ? maxSp : currentSp + skillPoints;
}

void Character::consumeSp(int skillPoints)
{
    if (skillPoints > 0 && hasEnoughSp(skillPoints))
        currentSp = (currentSp - skillPoints > 0) ? currentSp - skillPoints : 0;
}

void Character::addImmunity(const std::string &effectType)
{
    immunity.insert(effectType);
}

void Character::addImmunity(const std::set<std::string> &effectsType)
{
    immunity.insert(effectsType.begin(), effectsType.end());
}

void Character::removeImmunity(const std::string &effectType)
{
    immunity.erase(effectType);
}

void Character::removeImmunity(const std::set<std::string> &effectsType)
{
    immunity.erase(effectsType.begin(), effectsType.end());
}

// Combat Logic

void Character::takeDamage(int damage)
{
    float final_damage;
    if (defenseBoost == 0)
        final_damage = damage - (armor * 1);
    else
        final_damage = damage - (armor * (1 + defenseBoost / 100.0));

    final_damage = round(final_damage);
    if (final_damage >= 0)
        currentHp = (currentHp - final_damage > 0) ? currentHp - final_damage : 0;
}

void Character::heal(int amount)
{
    if (amount > 0)
        currentHp = (currentHp + amount > maxHp) ? maxHp : currentHp + amount;
}

void Character::applyDefense()
{
    defenseBoost = 40;
    setCooldown("Defense", 1);
}

void Character::addEffect(const Effect &e)
{
    std::string effectType = e.getType();
    for (size_t i = 0; i < effects.size(); i++)
    {
        if (effects[i].getType() == effectType)
        {
            int newDuration = e.getMaxDuration();
            if (effects[i].getMaxDuration() < newDuration)
            {
                effects[i].setDuration(newDuration);
            }
            else if (effects[i].getMaxDuration() == newDuration)
                effects[i].reset();
            return;
        }
    }
    effects.push_back(e);
}

void Character::processEffects()
{
    for (auto it = effects.begin(); it < effects.end();)
    {
        (*it).tick();

        if ((*it).isExpired() || attemptRemoveEffect(*it))
        {
            it = effects.erase(it);
        }
        else
        {
            takeDamage((*it).getDamagePerTurn());
            ++it;
        }
    }
}

bool Character::attemptRemoveEffect(const Effect &e)
{
    return e.tryRemoveEarly(attributes.constitution);
}

void Character::setCooldown(const std::string &skillName, int duration)
{
    cooldowns[skillName] = duration;
}

void Character::reduceCooldowns()
{
    for (auto it = cooldowns.begin(); it != cooldowns.end();)
    {
        it->second -= 1;

        if (it->second == 0)
        {
            if (it->first == "Defense")
                defenseBoost = 0;
            it = cooldowns.erase(it);
        }
        else
            ++it;
    }
}

// Accesors

bool Character::isDead() const
{
    return currentHp <= 0;
}

bool Character::hasEnoughSp(int sp) const
{
    return (currentSp - sp >= 0) ? true : false;
}

bool Character::canUseSkill(const std::string &skillName) const
{
    auto it = cooldowns.find(skillName);
    if (it != cooldowns.end())
        return false;
    return true;
}

bool Character::isImmune(const std::string &effectType) const
{
    if (immunity.find(effectType) != immunity.end())
        return true;
    return false;
}
// Output methods

void Character::display()
{
    // Upper part
    std::cout << std::string(20, '=') << "\n"
              << " NAME: " << name << "\n"
              << std::string(20, '=') << "\n"
              << " HP: " << currentHp << '/' << maxHp << "\t|\t"
              << " SP: " << currentSp << '/' << maxSp << "\t|\t"
              << " Armor: " << armor << "\n\n";

    // Attributes part
    std::cout << "ATTRIBUTES:" << "\n"
              << "\tStrength\t\t: " << attributes.strength << "\tAgility\t\t: " << attributes.agility
              << "\tConstitution\t: " << attributes.constitution << "\tIntelligence\t: " << attributes.intelligence
              << "\tWisdom\t\t: " << attributes.wisdom << "\tCharisma\t\t: " << attributes.charisma << "\n\n";

    // Effects part
    std::cout << "EFFECTS (active):" << "\n";
    for (const auto &effect : effects)
    {
        std::cout << "\t- " << effect.getType() << " (" << effect.getDuration() << ')';
        if (effect.getDamagePerTurn() > 0)
        {
            std::cout << " - damage: " << effect.getDamagePerTurn() << "/turn";
        }
        std::cout << "\n";
    }

    // Cooldowns
    std::cout << "COOLDOWNS:" << "\n";
    for (const auto &cooldown : cooldowns)
    {
        std::cout << "\t- " << cooldown.first << ": " << cooldown.second << " turns remaining" << "\n";
    }

    // End
    std::cout << std::string(20, '=') << "\n";
}

// Destructors