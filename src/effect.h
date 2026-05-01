#ifndef EFFECT_H
#define EFFECT_H

#ifndef NO_DIAGRAM
#include <string>
#endif

class Effect
{
private:
    /** @brief Effect's type */
    std::string type;

    /** @brief Effect's actual duration */
    int duration;

    /** @brief Effect's maximun duration */
    int maxDuration;

    /** @brief Effect's potency */
    int potency;

    /** @brief Flags the  */
    bool isNegative;

    /** @brief Effect's damage per turn */
    int damagePerTurn;

    public:

    // Constructor

    /** @brief Constructor.
     * \pre <em>true</em>
     * \post The result is a Effect.
     */
    Effect(const std::string& type, int duration, int potency, bool isNegative, int damagePerTurn);

    // Getters

    std::string getType() const;

    int getDuration() const;

    int getMaxDuration() const;

    int getPotency() const;

    bool getIsNegative() const;

    int getDamagePerTurn() const;

};

#endif