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
    Character(const std::string &name = "", int health = 0, int armor = 0);

    // Destructors

    virtual ~Character() noexcept = default;
};

#endif