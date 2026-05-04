# MiniRPG Console - Project Plan

This document is the primary reference for the project's design.
It should be updated whenever functionality is added, removed, or redefined.

---

## 1. Project Vision

Create a turn-based RPG minigame in a terminal to practice C++ fundamentals:
- OOP (inheritance, encapsulation, clear responsibilities)
- Safe state management in combat
- Deterministic logic + controlled random decisions
- Extensible foundation for future abilities and effects

---

## 2. Functional Goal (MVP)

Playable console version with:
- Player vs. one or multiple enemies
- Turn-based rounds: player first, then enemies
- Basic player actions: Attack, Defend, Heal
- Enemy AI: choose between attack, heal, or skill based on state
- Effect system (at least poison) with turn-based duration
- Cooldowns and skill point consumption

---

## 3. Current Functional Scope

### 3.1 General Character Attributes
- Name
- ActualHP
- MaxHP
- Armor
- Effects (multiple simultaneous)
- Skills
- Cooldowns for skills and effects
- SkillPoints
- Base attributes: Strength, Dexterity/Agility, Constitution/Health, Intelligence, Wisdom, Charisma

### 3.2 Player vs Enemy Differences
- Player:
  - Receives user input
  - Can defend by direct decision
  - Shows own information (health and skill points)
- Enemy:
  - Makes decisions with AI
  - Prioritizes healing when low on health
  - If no skill points for healing or ability, uses remaining valid options

### 3.3 States and Effects
- Duration is tracked by turns
- Poison damage is applied per turn
- A character can have multiple active effects simultaneously
- Reapplying the same effect type only refreshes it when the new maximum duration is equal to or greater than the existing one; equal maximum duration resets the active instance
- Early poison removal:
  - Probability based on resistance/constitution
  - Fewer turns remaining increases probability of removal

---

## 4. Conceptual Architecture

### 4.1 Responsibilities
- **Character** (base):
  - Common state (health, armor, resources, attributes, effects, cooldowns)
  - Common rules: take damage, heal, validate death, process states
- **Player**:
  - Choose action via validated input
- **Enemy**:
  - Choose action via weighted AI
- **Combat Controller** (in main or dedicated class):
  - Order turns and rounds
  - Apply chosen actions
  - Manage combat end conditions
  - Display result messages

### 4.2 Golden Rule
Combat logic must live in shared rules; Player and Enemy differ only in how they choose actions.

---

## 5. Terminal UX Policies

- Invalid input: does not consume turn
- Skill on cooldown or insufficient skill points: does not consume turn and requests action again
- Only valid action consumes turn
- Result messages after each action or effect tick

---

## 6. Base Enemy AI Decisions

1. Build list of valid actions (by cooldown and skill points)
2. If HP below threshold, increase healing priority
3. Choose action with weighted randomness
4. Resolve attack/ability failure based on target's dexterity/agility

---

## 7. Memory Management and Ownership Strategy

- Avoid manual new/delete
- Maintain unique ownership of combat entities
- Avoid multiple owners of the same character
- Combat controller operates on pre-created entities

---

## 8. Prioritized Backlog

1. Close Character data model (permanent vs. temporary state)
2. Define initial skill catalog
3. Define effect system (structure and application order)
4. Close mini combat specification
5. Implement complete combat loop
6. Add minimal tests for damage/cooldowns
7. Polish messages and terminal UX

---

## 9. Risks and Points to Watch

- Mixing turn logic with presentation logic
- Repeating damage/healing rules in derived classes
- Cooldown inconsistencies when action fails
- Not defining exact turn phase order

---

## 10. Mini Combat Specification v0.4 (FINAL)

This version closes all rules and parameters for implementation.

### 1. Active Combat Condition
- Combat continues while Player is alive and at least one Enemy is alive.

### 2. Round Order
- Player's turn
- All living enemies' turns (in fixed order)

### 3. Actor Turn Start
- Process active effects that tick per turn
- Attempt early effect removal with probability (if applicable)

### 4. Action Selection
- Player chooses via input
- Enemy chooses via AI from valid actions

### 4.1 AI Healing Priority Rule
- If AI.hp < 30% of its max health, prioritize healing action
- If cannot heal due to resources/cooldown, choose another valid action

### 5. Action Validation
- If no resources or on cooldown, action is invalid
- Invalid action does not consume Player's turn
- Enemy should never attempt invalid action if AI filters correctly

### 6. Offensive Attack/Ability Resolution
- **Hit/Miss**:
  - Generate random value r in range [0..99]
  - Target's agility is in percentage [0..100]
  - If r < target_agility, miss
  - Otherwise, hit
- If hits, apply damage mitigated by armor and active rules

### 6.1 Armor Mitigation (base)
- `Final_damage = (damage_base * attacker_strength_factor) - (armor_target * defense_factor_target)`
- If type_defense is not implemented in MVP, use default type_defense_target = 1.0
- Final damage cannot be negative

### 7. Healing Resolution
- Recovers HP without exceeding MaxHP
- Consumes resources and applies cooldown if applicable

### 8. Temporary Defense
- Increases mitigation for only 1 turn
- Used as tactical response against strong attacks requiring 2+ turns to prepare
- Temporary defense buff is +40%

### 9. Durable Effect Application
- If a skill applies a state, it enters with defined duration and potency
- Allows stacking of positive and negative effects
- Each effect (positive or negative) has its own turn counter (effect cooldown)
- When reaching 0 turns, the effect disappears

### 9.1 Early Removal of Negative Effects
- Negative effects can disappear earlier depending on type
- Fewer remaining turns increase removal probability
- Base probability depends on target's resistance:
  - Low resistance → low probability
  - High resistance → high probability
- Additionally, multiplied by temporal factor of the effect itself:
  - Fewer turns remaining to expire → higher cleanup factor

### 9.2 Immunities
- Player and Enemy can have immunity to specific negative effect types
- If an ability attempts to apply an effect the target is immune to, application fails
- If the turn's action was applying that effect, the turn is consumed and lost

### 9.3 Implementation Note on Temporal Factor
- Scaling is defined by remaining turns, not by effect potency
- Keep calculation bounded to avoid probabilities out of range

### 10. Cooldown Management
- Reduce all counters at end of actor's turn
- A cooldown becomes available when its counter reaches zero

### 11. Console Communication
- After each action/tick, inform of result: damage, healing, effect, miss, death
- Player's visible information takes priority over internal enemy data

### 12. End Condition
- If Player dies, defeat
- If all enemies die, victory

### 13. Strong Preparation Attacks
- A strong attack may require more than 2 turns to prepare
- While preparing, the game must show warning messages to Player
- For each turn elapsed, if less than X turns remain to execute the attack, show special alert message
- Value X is defined as a skill parameter

---

## 11. Combat Specification Progression

### 2026-04-30
- Project planning created
- Registered current decisions on attributes, effects, AI and action validation
- Added mini combat specification v0.1 as iterative base
- Updated to mini combat specification v0.2
- Closed rules for hit/miss, armor mitigation, effect stacking/removal, immunities and 1-turn temporary defense
- Updated to mini combat specification v0.3
- Closed parameters for AI by hp, hit/miss, defense buff (+40%), cleanup by resistance and alerts for strong attacks with preparation > 2 turns

### 2026-05-01
- Updated to mini combat specification v0.4 (FINAL)
- Closed RNG scale: random 0-99, agility in percentage (0-100%)

---

## 12. Design Decision Log

All major design decisions are documented in this plan.
For implementation details, see IMPLEMENTATION_PLAN.md

### 2026-05-04
- Confirmed `Attributes` is a standalone struct shared by Character, Player and Enemy
- Confirmed same-type effect reapplication resets the active effect when the incoming maximum duration matches the existing one
- Confirmed `display()` is a console-only presentation method and not part of combat rules
