#ifndef CHARACTER_H
#define CHARACTER_H

#ifndef NO_DIAGRAM
#include <string>
#endif

class Character
{
private:
    /** @brief Character's name */
    std::string name;

    /** @brief Character's actual health */
    int health;

    /** @brief Character's max health */
    int maxHp;

    /** @brief Character's armor */
    int armor;

public:
    // Constructors

    /** @brief Constructor by default.
     * \pre <em>true</em>
     * \post The result is a empty Character.
     */
    Character();

    /** @brief Constructor.
     * \pre <em>true</em>
     * \post The result is a Character.
     */
    Character(const std::string &name = "", int health = -1, int armor = 0);

    // Getters

    /** @brief Gets the Character's name.
     * \pre <em>true</em>
     * \post The result is the Character's name.
     */
    std::string getName() const;

    /** @brief Gets the Character's actual health.
     * \pre <em>true</em>
     * \post The result is the Character's actual health.
     */
    int getHealth() const;

    /** @brief Gets the Character's max health.
     * \pre <em>true</em>
     * \post The result is the Character's max health.
     */
    int getMaxHp() const;

    /** @brief Gets the Character's actual armor.
     * \pre <em>true</em>
     * \post The result is the Character's actual armor.
     */
    int getArmor() const;

    // Setters

    /** @brief Sets Character's health.
     * \pre An integer health
     * \post The result is a Character with an integer health.
     */
    int setHealth(int health);

    /** @brief Sets Character's armor.
     * \pre An integer armor
     * \post The result is a Character with an integer armor.
     */
    int setArmor(int armor);

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