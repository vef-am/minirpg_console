#ifndef CHARACTER_H
#define CHARACTER_H

#ifndef NO_DIAGRAM
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <set>
#endif

#include "effect.h"

struct Attributes
{
    int strength = 10;
    int agility = 10;
    int constitution = 10;
    int intelligence = 10;
    int wisdom = 10;
    int charisma = 10;
};

class Character
{
private:
    // Permanent (identity and progression)

    /** @brief Character's name */
    std::string name;

    /** @brief Character's current health */
    int currentHp;

    /** @brief Character's max health */
    int maxHp;

    /** @brief Character's armor */
    int armor;

    /** @brief Character's current skill points */
    int currentSp;

    /** @brief Character's max skill points */
    int maxSp;

    /** @brief Character base attributes */
    Attributes attributes;

    std::set<std::string> immunity;

    // Temporary (combat state)

    std::vector<Effect> effects;

    std::map<std::string, int> cooldowns;

    int defenseBoost;

public:
    // Constructors

    /** @brief Constructor by default.
     * \pre <em>true</em>
     * \post The result is a Character.
     */
    Character(const std::string &name, const Attributes &attributes = Attributes(), int currentHp = 50, int maxHp = 0, int armor = 10, int currentSp = 50, int maxSp = 0);

    // Getters

    /** @brief Gets the Character's name.
     * \pre <em>true</em>
     * \post The result is the Character's name.
     */
    std::string getName() const;

    /** @brief Gets the Character's current health.
     * \pre <em>true</em>
     * \post The result is the Character's current health.
     */
    int getCurrentHp() const;

    /** @brief Gets the Character's max health.
     * \pre <em>true</em>
     * \post The result is the Character's max health.
     */
    int getMaxHp() const;

    /** @brief Gets the Character's current armor.
     * \pre <em>true</em>
     * \post The result is the Character's current armor.
     */
    int getArmor() const;

    /** @brief Gets the Character's current skill points.
     * \pre <em>true</em>
     * \post The result is the Character's current skill points.
     */
    int getCurrentSp() const;

    /** @brief Gets the Character's max skill points.
     * \pre <em>true</em>
     * \post The result is the Character's max skill points.
     */
    int getMaxSp() const;

    /** @brief Gets the Character's strength.
     * \pre <em>true</em>
     * \post The result is the Character's strength.
     */
    int getStrength() const;

    /** @brief Gets the Character's agility.
     * \pre <em>true</em>
     * \post The result is the Character's agility.
     */
    int getAgility() const;

    /** @brief Gets the Character's constitution.
     * \pre <em>true</em>
     * \post The result is the Character's constitution.
     */
    int getConstitution() const;

    /** @brief Gets the Character's intelligence.
     * \pre <em>true</em>
     * \post The result is the Character's intelligence.
     */
    int getIntelligence() const;

    /** @brief Gets the Character's wisdom.
     * \pre <em>true</em>
     * \post The result is the Character's wisdom.
     */
    int getWisdom() const;

    /** @brief Gets the Character's charisma.
     * \pre <em>true</em>
     * \post The result is the Character's charisma.
     */
    int getCharisma() const;

    /** @brief Gets the Character's effects.
     * \pre <em>true</em>
     * \post The result is the Character's effects.
     */
    const std::vector<Effect> &getEffects() const;

    /** @brief Tells if the Character is alive.
     * \pre <em>true</em>
     * \post The result is if the Character is alive or not.
     */
    bool isAlive() const;

    // Setters

    /** @brief Sets Character's health.
     * \pre An integer health
     * \post The result is a Character with an integer health.
     */
    void setHealth(int health);

    /** @brief Sets Character's armor.
     * \pre An integer armor
     * \post The result is a Character with an integer armor.
     */
    void setArmor(int armor);

    void addSp(int skillPoints);

    void consumeSp(int skillPoints);

    void addImmunity(const std::string& effectType);

    void addImmunity(const std::set<std::string> &effectsType);

    void removeImmunity(const std::string& effectType);

    void removeImmunity(const std::set<std::string> &effectsType);

    // Combat Logic

    void takeDamage(int damage);

    void heal(int amount);

    void applyDefense();

    void addEffect(const Effect &e);

    void processEffects();

    bool attemptRemoveEffect(const Effect &e);

    void setCooldown(const std::string &skillName, int duration);

    void reduceCooldowns();

    // Accessors

    /** @brief Tells if the Character is dead.
     * \pre <em>true</em>
     * \post The result is if the Character is dead or not.
     */
    bool isDead() const;

    bool hasEnoughSp(int sp) const;

    bool canUseSkill(const std::string &skillName) const;

    bool isImmune(const std::string& effectType) const;

    // Output methods

    void display();

    // Destructors

    virtual ~Character() noexcept = default;
};

#endif