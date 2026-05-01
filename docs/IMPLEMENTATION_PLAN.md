# Implementation Plan - MiniRPG Console

This document details what each file/module needs according to specification v0.4.

---

## 1. Character.h/cpp - Base Model

### Responsibility
Core of shared combat mechanics between Player and Enemy.

### Required Attributes

#### Permanent (identity and progression)
- `std::string name`
- `int maxHp` - maximum health
- `int currentHp` - current health
- `int armor` - physical mitigation
- `int skillPoints` - skill points available
- `int maxSkillPoints` - maximum skill points

#### Base Attributes (6 pillars)
- `int strength` - physical damage
- `int agility` - evasion and quick attack
- `int constitution` - resistance to effects
- `int intelligence` - magical damage
- `int wisdom` - healing and psychic resistance
- `int charisma` - (reserved for expansion)

#### Temporary (combat state)
- `std::vector<Effect> effects` - list of active effects
- `std::map<std::string, int> cooldowns` - cooldowns by skill name
- `int defenseBoost` - +40% temporary defense (duration: 1 turn)

### Required Methods

#### Getters
- `getName()`, `getMaxHp()`, `getCurrentHp()`, `getArmor()`, `getSkillPoints()`
- `getStrength()`, `getAgility()`, `getConstitution()`, `getIntelligence()`, `getWisdom()`, `getCharisma()`
- `getEffects()` - returns const ref to effects
- `isAlive()` - opposite of isDead()

#### Setters
- `setHealth(int)` - updates currentHp respecting [0..maxHp]
- `setArmor(int)`
- `addSkillPoints(int)` / `consumeSkillPoints(int)` - skill point management

#### Combat Logic
- `takeDamage(int damage)` - applies armor mitigation and defenseBoost formula
- `heal(int amount)` - recovers HP without exceeding maxHp
- `applyDefense()` - activates defenseBoost = +40% for 1 turn
- `addEffect(const Effect& e)` - adds effect to list
- `processEffects()` - applies active effect damage and reduces duration
- `attemptRemoveEffect(const Effect& e)` - probability of early removal based on constitution
- `setCooldown(const std::string& skillName, int duration)` - sets cooldown
- `reduceCooldowns()` - reduces all cooldowns by 1 turn
- `canUseSkill(const std::string& skillName)` - checks cooldown and skill points
- `isImmune(const std::string& effectType)` - checks character immunity
- `isDead()` - returns true if currentHp <= 0

#### Utility
- `display()` - shows name, HP, armor (and more if needed)

### Design Considerations
- `takeDamage()` must apply formula: `final_damage = (damage * attacker_strength_factor) - (armor * defense_factor)`
- Armor cannot reduce damage to negative
- `applyDefense()` only modifies defenseBoost for 1 turn (resets at turn end)
- Effects must be stackable (multiple can coexist)

---

## 2. Effect.h - Effect Structure

### Responsibility
Represent an active effect with its type, duration, potency and associated immunities.

### Attributes
- `std::string type` - effect type (Poison, Stun, Buff, etc.)
- `int duration` - remaining turns
- `int potency` - effect intensity
- `bool isNegative` - flag to differentiate negative from positive
- `int damagePerTurn` - damage applied this turn (only if negative)
- `std::vector<std::string> immuneCharacters` - characters that can be immune to this type

### Methods
- Constructor
- `tick()` - reduces duration by 1
- `isExpired()` - returns true if duration <= 0
- `canBeRemovedEarly(int constitutionOfTarget)` - calculates early removal probability

### Considerations
- Early removal depends on:
  - Target's constitution (high constitution = higher probability)
  - Remaining duration (fewer turns = higher probability)
- Formula: `prob = (resistance / 100) * ((max_duration - remaining_turns) / max_duration)`

---

## 3. Player.h/cpp - Controlled Protagonist

### Responsibility
Validated user input, manual decision-making.

### Inherits from Character
- All base attributes and methods

### Player-Specific Methods
- `takeTurn()` - input loop until valid action:
  1. Show options: Attack, Defend, Heal, Skill1, Skill2...
  2. Validate input
  3. Validate resources (cooldown, skill points)
  4. If invalid, request again without consuming turn
  5. Return chosen action
- `executeAction(const std::string& action)` - executes chosen action
- `displayStatus()` - shows name, HP current/max, SP current/max, active effects

### Differences from Enemy
- **Human input**: direct input/output with user
- **Interactive validation**: rejects invalid actions WITHOUT consuming turn
- **Visible information**: Player always sees complete status

---

## 4. Enemy.h/cpp - AI-Controlled Adversary

### Responsibility
Decide action with weighted logic, without user input.

### Inherits from Character
- All base attributes and methods

### Enemy-Specific Methods
- `takeTurn()` - chooses action based on AI:
  1. If currentHp < 30% * maxHp → prioritize Heal
  2. Build list of valid actions (filtered by cooldown/SP)
  3. If no valid Heal, choose randomly from valid actions
  4. Resolve attack probability (miss by Player's agility)
- `chooseAction(const Character& player)` - returns string with chosen action
- `attemptAttack(const Character& target)` - checks hit/miss and applies damage
- `displayStatus()` - shows name, HP (can be hidden if design allows)

### AI Logic
```
if (currentHp < 30% of maxHp):
  if (canUseSkill("Heal")):
    use Heal
  else:
    choose random valid action
else:
  choose weighted random action
```

### Hit/Miss
- Generate `random [0..99]`
- If `random < target.getAgility()` → miss
- Otherwise → hit

---

## 5. Combat.h/cpp (new class) - Turn Orchestrator

### Responsibility
Manage combat loop, turns, rounds and end conditions.

### Attributes
- `Character& player`
- `std::vector<Character*> enemies`
- `int currentRound`
- `bool combatActive`

### Methods
- `startCombat()` - initializes combat, shows initial state
- `runRound()` - executes player turn, then all enemies:
  1. Player.takeTurn() → chosen action
  2. For each living Enemy:
     - Enemy.takeTurn() → chosen action
     - Execute action (attack, healing, defense)
  3. Process effects (apply damage, reduce duration)
  4. Show result messages
  5. Reduce cooldowns
  6. Check end condition
- `executeAction(Character& actor, const std::string& action, Character* target)` - applies action mechanics
- `isGameOver()` - returns true if player dead or all enemies dead
- `displayResults()` - shows combat result

### Turn Processing Order
1. Start: process active effects (damage per turn)
2. Select action (Player input or Enemy AI)
3. Validate action (cooldown, SP)
4. Execute action (attack with hit/miss, healing, defense)
5. Apply secondary effects (apply new effect if skill)
6. Show messages
7. Reduce cooldown counters

---

## 6. Skills.h/cpp - Ability Catalog

### Responsibility
Define available abilities with their parameters (cost, cooldown, damage, effects).

### Skill Structure
- `std::string name` - ability name
- `int skillPointsCost` - cost in points
- `int cooldownDuration` - cooldown duration
- `int damage` - base damage (if offensive)
- `int healAmount` - healing (if defensive)
- `Effect* appliedEffect` - effect that grants (if applicable)
- `int preparationTurns` - preparation turns (for strong attacks)
- `bool targetNeedsCast` - if target validation needed (strong attacks)

### Methods
- `execute(Character& actor, Character* target)` - applies the ability

### Initial MVP Abilities
1. **Attack** - base attack, no cooldown, cost 0
2. **Defend** - temporary defense +40%, no cooldown, cost 0
3. **Heal** - recovers 30 HP, cooldown 2 turns, cost 10 SP
4. **Poison Strike** - attack + applies Poison (10 damage/turn, 3 turns), cooldown 3, cost 15 SP

---

## 7. main.cpp - Entry Point and UI

### Responsibility
Initialize game, create entities, execute combat loop.

### Structure
```
1. Create Player (initial stats)
2. Create Enemy/Enemies (initial stats)
3. Show initial state
4. Instantiate Combat
5. Loop: while combat active
   - runRound()
   - Show state after round
6. Show final result (victory/defeat)
```

### Considerations
- Use `unique_ptr` for Player and `vector<unique_ptr<Enemy>>` for enemies
- Avoid raw `new`/`delete`

---

## 8. Proposed File Structure

```
src/
├── Character.h / Character.cpp    (base, attributes, common mechanics)
├── Effect.h / Effect.cpp          (effect structure)
├── Player.h / Player.cpp          (player with input)
├── Enemy.h / Enemy.cpp            (enemy with AI)
├── Combat.h / Combat.cpp          (turn orchestrator)
├── Skills.h / Skills.cpp          (ability catalog)
└── main.cpp                        (entry point, terminal UI)
```

---

## 9. Implementation Dependencies (recommended order)

1. **Effect.h/cpp** - no external dependencies
2. **Character.h/cpp** - depends on Effect
3. **Skills.h/cpp** - depends on Character (optional: may not depend directly)
4. **Player.h/cpp** - depends on Character
5. **Enemy.h/cpp** - depends on Character
6. **Combat.h/cpp** - depends on Player, Enemy, Skills
7. **main.cpp** - depends on Combat

---

## 10. Validations and Edge Cases

### In Character
- HP never < 0 or > maxHp
- SkillPoints never < 0
- Armor never < 0
- Agility in range [0..100]

### In Combat
- Don't process turns of dead enemies
- Reduce cooldowns only if action was valid
- If enemy attempts skill on cooldown, turn is lost (no valid action)

### In Player
- Invalid input doesn't consume turn
- Skill on cooldown or insufficient SP: message and request again

---

## 11. Open Decision Points (to be determined)

1. Show or hide enemy HP?
2. Show "attack preparation" messages only if player sees it or always?
3. Combat speed (pauses between actions)?
4. Animations or text only?

---

## 12. Change Log

### 2026-05-01
- Created technical implementation plan
- Detailed responsibilities and methods per file
- Defined data structures for Character, Effect, Player, Enemy, Combat, Skills
- Recommended implementation order
- Translated to English for public repository

---

## 13. Notes for Implementation

- Always maintain the "Golden Rule": combat logic lives in shared rules
- Player and Enemy inherit from Character for code reuse
- UI messages should be clear and consistent
- Consider using enums for action types and effect types
- Use const references where appropriate to avoid unnecessary copies
