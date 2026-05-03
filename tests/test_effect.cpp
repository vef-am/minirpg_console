#include "../src/effect.h"

#include <cassert>
#include <iostream>

namespace
{
    void test_creation_and_getters()
    {
        Effect poison("Poison", 4, 7, true, 3);

        assert(poison.getType() == "Poison");
        assert(poison.getDuration() == 4);
        assert(poison.getMaxDuration() == 4);
        assert(poison.getPotency() == 7);
        assert(poison.getIsNegative());
        assert(poison.getDamagePerTurn() == 3);
        assert(!poison.isExpired());
    }

    void test_tick_and_expiration()
    {
        Effect poison("Poison", 4, 7, true, 3);

        poison.tick();
        assert(poison.getDuration() == 3);
        assert(!poison.isExpired());

        poison.tick();
        assert(poison.getDuration() == 2);
        assert(!poison.isExpired());

        poison.tick();
        assert(poison.getDuration() == 1);
        assert(!poison.isExpired());

        poison.tick();
        assert(poison.getDuration() == 0);
        assert(poison.isExpired());
    }

    void test_reset_and_set_duration()
    {
        Effect poison("Poison", 4, 7, true, 3);

        poison.tick();
        poison.tick();
        assert(poison.getDuration() == 2);

        poison.reset();
        assert(poison.getDuration() == 4);
        assert(poison.getMaxDuration() == 4);

        poison.setDuration(6);
        assert(poison.getDuration() == 6);
        assert(poison.getMaxDuration() == 6);

        poison.tick();
        assert(poison.getDuration() == 5);
        poison.reset();
        assert(poison.getDuration() == 6);
    }

    void test_positive_effect_never_removes_early()
    {
        Effect blessing("Blessing", 3, 2, false, 0);

        for (int constitution = 0; constitution <= 100; constitution += 25)
        {
            assert(!blessing.tryRemoveEarly(constitution));
        }
    }

    void test_negative_effect_removal_trend_by_constitution()
    {
        Effect lowConstitutionPoison("Poison", 5, 1, true, 1);
        Effect highConstitutionPoison("Poison", 5, 1, true, 1);

        lowConstitutionPoison.tick();
        highConstitutionPoison.tick();

        int lowRemovedCount = 0;
        int highRemovedCount = 0;
        for (int i = 0; i < 200; ++i)
        {
            if (lowConstitutionPoison.tryRemoveEarly(10))
            {
                ++lowRemovedCount;
            }

            if (highConstitutionPoison.tryRemoveEarly(90))
            {
                ++highRemovedCount;
            }
        }

        assert(lowRemovedCount <= highRemovedCount);
        assert(highRemovedCount > 0);
    }

    void test_negative_effect_removal_trend_by_duration()
    {
        Effect earlyPoison("Poison", 5, 1, true, 1);
        Effect latePoison("Poison", 5, 1, true, 1);

        earlyPoison.tick();
        latePoison.tick();
        latePoison.tick();
        latePoison.tick();

        int earlyRemovedCount = 0;
        int lateRemovedCount = 0;
        for (int i = 0; i < 200; ++i)
        {
            if (earlyPoison.tryRemoveEarly(80))
            {
                ++earlyRemovedCount;
            }

            if (latePoison.tryRemoveEarly(80))
            {
                ++lateRemovedCount;
            }
        }

        assert(earlyRemovedCount <= lateRemovedCount);
        assert(lateRemovedCount > 0);
    }

    void test_expiration_after_last_tick()
    {
        Effect poison("Poison", 1, 7, true, 3);
        assert(!poison.isExpired());

        poison.tick();
        assert(poison.getDuration() == 0);
        assert(poison.isExpired());
    }
} // namespace

int main()
{
    std::cout << "Running Effect unit tests...\n";

    test_creation_and_getters();
    test_tick_and_expiration();
    test_reset_and_set_duration();
    test_positive_effect_never_removes_early();
    test_negative_effect_removal_trend_by_constitution();
    test_negative_effect_removal_trend_by_duration();
    test_expiration_after_last_tick();

    std::cout << "All Effect tests passed\n";
    return 0;
}