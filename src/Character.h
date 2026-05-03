#ifndef CHARACTER_H
#define CHARACTER_H

#ifndef NO_DIAGRAM
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
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
    Character(const std::string &name, const Attributes &attributes = Attributes(), int currentHp = 0, int maxHp = 0, int armor = 0, int currentSp = 0, int maxSp = 0);

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

    int getStrength() const;

    int getAgility() const;

    int getConstitution() const;

    int getIntelligence() const;

    int getWisdom() const;

    int getCharisma() const;

    const std::vector<Effect> &getEffects() const;

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

    // Modifiers

    /** @brief Sets Character's armor
     * \pre An integer
     * \post The result is a Character with a armor.
     */
    int takeDamage(int damage);

    int heal(int amount);

    // Accessors

    /** @brief Tells if the Character is dead.
     * \pre <em>true</em>
     * \post The result is if the Character is dead or not.
     */
    bool isDead() const;

    // Output methods

    void display();

    // Destructors

    virtual ~Character() noexcept = default;
};

#endif