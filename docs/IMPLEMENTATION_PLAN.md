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
- `std::set<std::string> immunity` - effect types the character cannot receive

### Required Methods

#### Getters
- `getName()`, `getMaxHp()`, `getCurrentHp()`, `getArmor()`, `getSkillPoints()`
- `getStrength()`, `getAgility()`, `getConstitution()`, `getIntelligence()`, `getWisdom()`, `getCharisma()`
- `getEffects()` - returns const ref to effects
- `isAlive()` - opposite of isDead()

#### Setters
- `setHealth(int)` - updates currentHp respecting [0..maxHp]
- `setArmor(int)`
- `addSp(int)` / `consumeSp(int)` - skill point management
- `addImmunity(const std::string&)`, `addImmunity(const std::set<std::string>&)`
- `removeImmunity(const std::string&)`, `removeImmunity(const std::set<std::string>&)`

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
- `display()` - console-only presentation of HP, SP, armor, attributes, effects and cooldowns

### Design Considerations
- `takeDamage()` must apply formula: `final_damage = (damage * attacker_strength_factor) - (armor * defense_factor)`
- Armor cannot reduce damage to negative
- `applyDefense()` only modifies defenseBoost for 1 turn (resets at turn end)
- Effects must be stackable (multiple can coexist)
- Reapplying an effect of the same type only refreshes/reset the active instance when the incoming maximum duration is equal to or greater than the one already stored; otherwise, keep the existing effect unchanged

### 1.1 Implementation notes for developer (Character behavior)

Purpose: `Character` is the shared combat core. Implement it first as a clean, stable base before adding `Player` and `Enemy`.

Recommended implementation order:

1. Constructor and invariants
  - Decide what the default constructor means for an "empty" character.
  - Ensure `maxHp`, `health`, and `armor` stay in valid ranges.
  - If `health > maxHp`, clamp it.
  - If `health < 0` or `armor < 0`, clamp them to 0.

2. Basic getters
  - Implement and verify name, health, maxHp, and armor first.
  - These are the easiest checks and help confirm the constructor is correct.

3. Setters and core state mutators
  - Define clear semantics for `setHealth(int)` and `setArmor(int)`.
  - If you keep their current `int` return type, return the final stored value so the caller can see the clamped result.
  - If you later prefer a `void` API, change the header and cpp together consistently.

4. Damage and healing rules
  - `takeDamage(int)` should be the main place where armor and temporary defense are applied.
  - `heal(int)` should never raise health above maxHp.
  - Keep both methods focused on one responsibility.

5. Death checks and display
  - `isDead()` should only depend on current health.
  - `display()` should be presentation only; avoid combat logic there.

6. Effects and cooldowns later
  - Do not mix effect lifecycle or cooldown logic into the first pass unless the data structures are already in place.
  - Add them after the base health/armor behavior is stable.

Suggested test checklist for Character:

- Constructor initializes values within bounds.
- `setHealth()` clamps below 0 and above maxHp.
- `setArmor()` clamps below 0.
- `takeDamage()` reduces health correctly.
- `takeDamage()` never drops health below 0.
- `heal()` restores health correctly.
- `heal()` never exceeds maxHp.
- `isDead()` returns true only at 0 HP.
- `display()` is optional for automated tests; manual inspection is enough.

Current codebase notes:
- `src/character.cpp` currently has the base HP/SP, damage, effects, cooldown and immunity APIs in place.
- `display()` is presentation-only and should stay free of combat rules.
- Keep constructor logic and setter logic aligned; otherwise the class will validate values in one place but not the other.
- Keep effect reapplication aligned with the max-duration rule above.

---

## 2. Effect.h/cpp - Effect Structure

### Responsibility
Represent an active effect with its type, duration, potency and intensity. Effects are mutable and live inside a `Character`'s collection; when the same effect type is reapplied with longer duration, it replaces the existing one.

### Required Attributes
- `std::string type` - effect identifier (Poison, Stun, DefenseBuff, etc.)
- `int duration` - remaining turns
- `int maxDuration` - original duration at application (used for probability calculations and resets)
- `int potency` - effect intensity (damage per turn, buff amount, etc.)
- `bool isNegative` - true if effect is harmful
- `int damagePerTurn` - damage applied each turn (only for damage-over-time effects)

### Required Methods

#### Getters
- `getType() const` - returns effect type
- `getDuration() const` - returns remaining turns
- `getMaxDuration() const` - returns original duration
- `getPotency() const` - returns intensity value
- `getIsNegative() const` - returns whether effect is harmful
- `getDamagePerTurn() const` - returns damage per turn

#### Lifecycle methods
- Constructor: `Effect(const std::string &type, int duration, int potency, bool isNegative, int damagePerTurn)`
- `tick()` - reduces duration by 1
- `isExpired() const` - returns true if duration <= 0
- `reset()` - sets duration back to maxDuration (used when reapplying effect)
- `setDuration(int d)` - updates duration (useful when comparing durations on reapplication)

#### Effect evaluation
- `tryRemoveEarly(int constitution) const` - calculates early removal probability based on:
  - Target's constitution (0..100): higher constitution = higher removal probability
  - Remaining duration: fewer turns remaining = higher removal probability
  - Formula: `probability = (constitution / 100.0) * ((maxDuration - duration) / maxDuration)`
  - Should return true if effect should be removed this turn

### Design Considerations
- Effects are mutable and stored in `std::vector<Effect>` within Character
- When an effect type is reapplied:
  - If new duration > existing duration: replace existing effect with new one
  - If new duration <= existing duration: keep existing effect unchanged
  - Example: Poison(5 turns) replaces Poison(2 turns), but Poison(2 turns) does not replace Poison(5 turns)
- Early removal is probabilistic and only applies to negative effects
- The `maxDuration` field allows consistent probability calculations even after duration has been reduced by ticks

### 2.2 Testing suggestions for Effect (write in tests/test_effect.cpp)

**Basic lifecycle tests:**
1. Effect creation: verify all fields are initialized correctly (type, duration, maxDuration, potency, isNegative, damagePerTurn)
2. Getters: all getters return correct values
3. tick() behavior: duration decreases by 1 each call, and isExpired() transitions correctly

**Duration management tests:**
4. isExpired() returns false while duration > 0, true when duration == 0
5. reset() restores duration to maxDuration
6. setDuration(d) updates both duration and maxDuration to d (reapplication scenario)

**Early removal tests:** 
7. Positive effects (isNegative=false) never trigger early removal, always return false
8. Negative effects (isNegative=true) with constitution=0 should never remove (probability=0)
9. Negative effects with high constitution (80-100) and duration close to expiry (duration near 0) should have high removal probability
   - Hint: You need to tick() the effect first so (maxDuration - duration) > 0
10. Probability calculation: lower constitution = lower removal chance; this is probabilistic so test with multiple calls

**Edge case tests:**
11. Effect with duration=1: tick once and verify it expires
12. Effect with maxDuration=1: verify probability formula doesn't divide by zero (should be 0 since maxDuration-duration always 0)
13. Reapplication scenario: Apply Poison(2), tick once (duration=1), then reapply with Poison(5). Verify new duration is 5 and maxDuration is 5.

**Compilation:** 
```bash
g++ -std=c++17 src/effect.cpp tests/test_effect.cpp -o tests/test_effect
./tests/test_effect
```

All tests should pass and exit with code 0.

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
