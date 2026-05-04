#include "../src/character.h"

#include <cassert>
#include <iostream>
#include <string>

namespace
{
    void test_constructor_and_getters()
    {
        Attributes attributes;
        attributes.strength = 12;
        attributes.agility = 14;
        attributes.constitution = 16;
        attributes.intelligence = 18;
        attributes.wisdom = 20;
        attributes.charisma = 22;

        Character hero("Hero", attributes, 60, 80, 5, 30, 40);

        assert(hero.getName() == "Hero");
        assert(hero.getCurrentHp() == 60);
        assert(hero.getMaxHp() == 80);
        assert(hero.getArmor() == 5);
        assert(hero.getCurrentSp() == 30);
        assert(hero.getMaxSp() == 40);
        assert(hero.getStrength() == 12);
        assert(hero.getAgility() == 14);
        assert(hero.getConstitution() == 16);
        assert(hero.getIntelligence() == 18);
        assert(hero.getWisdom() == 20);
        assert(hero.getCharisma() == 22);
        assert(hero.isAlive());
        assert(!hero.isDead());

        std::cout << "  constructor_and_getters ok\n";
    }

    void test_constructor_invariants()
    {
        Character hero("Clamped", Attributes(), 50, 30, -5, 20, 10);
        assert(hero.getCurrentHp() == 30);
        assert(hero.getMaxHp() == 30);
        assert(hero.getArmor() == 0);
        assert(hero.getCurrentSp() == 10);
        assert(hero.getMaxSp() == 10);

        Character fallback("Fallback", Attributes(), 45, 0, 2, 15, 0);
        assert(fallback.getMaxHp() == 45);
        assert(fallback.getCurrentHp() == 45);
        assert(fallback.getMaxSp() == 15);
        assert(fallback.getCurrentSp() == 15);

        std::cout << "  constructor_invariants ok\n";
    }

    void test_health_and_armor_setters()
    {
        Character hero("Setter", Attributes(), 40, 50, 8, 12, 20);

        hero.setHealth(60);
        assert(hero.getCurrentHp() == 50);

        hero.setHealth(-10);
        assert(hero.getCurrentHp() == 0);

        hero.setHealth(25);
        assert(hero.getCurrentHp() == 25);

        hero.setArmor(-4);
        assert(hero.getArmor() == 0);

        hero.setArmor(11);
        assert(hero.getArmor() == 11);

        std::cout << "  health_and_armor_setters ok\n";
    }

    void test_skill_points_api()
    {
        Character hero("SP", Attributes(), 40, 50, 8, 12, 20);

        assert(hero.hasEnoughSp(10));
        assert(!hero.hasEnoughSp(13));

        hero.addSp(15);
        assert(hero.getCurrentSp() == 20);

        hero.addSp(-3);
        assert(hero.getCurrentSp() == 20);

        hero.consumeSp(7);
        assert(hero.getCurrentSp() == 13);

        hero.consumeSp(99);
        assert(hero.getCurrentSp() == 13);

        hero.consumeSp(-2);
        assert(hero.getCurrentSp() == 13);

        std::cout << "  skill_points_api ok\n";
    }

    void test_damage_heal_and_defense()
    {
        Character hero("Tank", Attributes(), 100, 100, 10, 10, 20);

        hero.takeDamage(30);
        assert(hero.getCurrentHp() == 80);

        hero.heal(15);
        assert(hero.getCurrentHp() == 95);

        hero.heal(999);
        assert(hero.getCurrentHp() == 100);

        hero.takeDamage(200);
        assert(hero.getCurrentHp() == 0);

        Character defender("Shield", Attributes(), 100, 100, 10, 10, 20);
        defender.applyDefense();
        defender.takeDamage(30);
        assert(defender.getCurrentHp() == 84);
        defender.reduceCooldowns();
        defender.takeDamage(30);
        assert(defender.getCurrentHp() == 64);

        std::cout << "  damage_heal_and_defense ok\n";
    }

    void test_effect_lifecycle_and_reapplication()
    {
        Character hero("EffectUser", Attributes(), 50, 50, 0, 10, 10);

        Effect poison("Poison", 3, 4, true, 5);
        hero.addEffect(poison);
        assert(hero.getEffects().size() == 1);
        assert(hero.getEffects()[0].getType() == "Poison");
        assert(hero.getEffects()[0].getDuration() == 3);
        assert(hero.getEffects()[0].getMaxDuration() == 3);

        Effect weakerPoison("Poison", 2, 6, true, 7);
        hero.addEffect(weakerPoison);
        assert(hero.getEffects().size() == 1);
        assert(hero.getEffects()[0].getDuration() == 3);
        assert(hero.getEffects()[0].getMaxDuration() == 3);
        assert(hero.getEffects()[0].getPotency() == 4);
        assert(hero.getEffects()[0].getDamagePerTurn() == 5);

        Effect sameDurationPoison("Poison", 3, 9, true, 11);
        hero.addEffect(sameDurationPoison);
        assert(hero.getEffects().size() == 1);
        assert(hero.getEffects()[0].getDuration() == 3);
        assert(hero.getEffects()[0].getMaxDuration() == 3);
        assert(hero.getEffects()[0].getPotency() == 4);
        assert(hero.getEffects()[0].getDamagePerTurn() == 5);

        Effect strongerPoison("Poison", 5, 8, true, 2);
        hero.addEffect(strongerPoison);
        assert(hero.getEffects().size() == 1);
        assert(hero.getEffects()[0].getType() == "Poison");
        assert(hero.getEffects()[0].getDuration() == 5);
        assert(hero.getEffects()[0].getMaxDuration() == 5);
        assert(hero.getEffects()[0].getPotency() == 8);
        assert(hero.getEffects()[0].getDamagePerTurn() == 2);

        Effect blessing("Blessing", 2, 1, false, 0);
        hero.addEffect(blessing);
        assert(hero.getEffects().size() == 2);

        std::cout << "  effect_lifecycle_and_reapplication ok\n";
    }

    void test_process_effects()
    {
        Character hero("EffectTick", Attributes(), 50, 50, 0, 10, 10);
        Effect burn("Burn", 2, 1, false, 5);
        hero.addEffect(burn);

        hero.processEffects();
        assert(hero.getCurrentHp() == 45);
        assert(hero.getEffects().size() == 1);
        assert(hero.getEffects()[0].getDuration() == 1);

        hero.processEffects();
        assert(hero.getCurrentHp() == 40);
        assert(hero.getEffects().empty());

        std::cout << "  process_effects ok\n";
    }

    void test_cooldowns_and_immunity()
    {
        Character hero("Cooldown", Attributes(), 50, 50, 10, 10, 10);

        assert(hero.canUseSkill("Defense"));
        hero.setCooldown("Defense", 1);
        assert(!hero.canUseSkill("Defense"));
        hero.reduceCooldowns();
        assert(hero.canUseSkill("Defense"));

        assert(!hero.isImmune("Poison"));
        hero.addImmunity("Poison");
        assert(hero.isImmune("Poison"));
        hero.removeImmunity("Poison");
        assert(!hero.isImmune("Poison"));

        std::set<std::string> immunities = {"Sleep", "Stun"};
        hero.addImmunity(immunities);
        assert(hero.isImmune("Sleep"));
        assert(hero.isImmune("Stun"));
        hero.removeImmunity(immunities);
        assert(!hero.isImmune("Sleep"));
        assert(!hero.isImmune("Stun"));

        std::cout << "  cooldowns_and_immunity ok\n";
    }
} // namespace

int main()
{
    std::cout << "Running Character unit tests...\n";

    test_constructor_and_getters();
    test_constructor_invariants();
    test_health_and_armor_setters();
    test_skill_points_api();
    test_damage_heal_and_defense();
    test_effect_lifecycle_and_reapplication();
    test_process_effects();
    test_cooldowns_and_immunity();

    std::cout << "All Character tests passed\n";
    return 0;
}
