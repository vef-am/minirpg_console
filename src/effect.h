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

    /** @brief Flags if the Effect is negative or not */
    bool isNegative;

    /** @brief Effect's damage per turn */
    int damagePerTurn;

public:
    // Constructor

    /** @brief Constructor.
     * \pre <em>true</em>
     * \post The result is an Effect.
     */
    Effect(const std::string &type, int duration, int potency, bool isNegative, int damagePerTurn);

    // Getters

    /** @brief Gets the Effect's type.
     * \pre <em>true</em>
     * \post The result is the Effect's type.
     */
    std::string getType() const;

    /** @brief Gets the Effect's duration.
     * \pre <em>true</em>
     * \post The result is the Effect's duration.
     */
    int getDuration() const;

    /** @brief Gets the Effect's maximun duration.
     * \pre <em>true</em>
     * \post The result is the Effect's maximun duration.
     */
    int getMaxDuration() const;

    /** @brief Gets the Effect's potency.
     * \pre <em>true</em>
     * \post The result is the Effect's potency.
     */
    int getPotency() const;

    /** @brief Tells if the Effect is negative or not.
     * \pre <em>true</em>
     * \post The result is true for negative or false for positive.
     */
    bool getIsNegative() const;

    /** @brief Gets the Effect's damage per turn.
     * \pre <em>true</em>
     * \post The result is the Effect's damage per turn.
     */
    int getDamagePerTurn() const;

    // Methods

    /** @brief Reduces duration by 1.
     * \pre duration > 0
     * \post The Effect's duration is reduced by 1.
     */
    void tick();

    /** @brief Tells if the Effect is expired or not.
     * \pre <em>true</em>
     * \post The result is true for expired, false for not.
     */
    bool isExpired() const;

    /** @brief Resets the effect in case of reapplied.
     * \pre <em>true</em>
     * \post The result is a reseted Effect.
     */
    void reset();

    /** @brief Updates duration.
     * \pre A duration d > 0.
     * \post The result is an Effect with an updated duration.
     */
    void setDuration(int d);
};

#endif