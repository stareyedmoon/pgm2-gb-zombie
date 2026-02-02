#include <stdint.h>
#include <stdbool.h>

#include <game.h>
#include <entity.h>

#ifndef _game_encounter_h_INCLUDE
#define _game_encounter_h_INCLUDE


/// @brief Effective stats for an entity. No need to calculate them every time we need to use them.
typedef struct {
    uint8_t intelligence;
    uint8_t strength;
    uint8_t speed;

    bool can_use_weapon;
} EffectiveStats;

/// @brief All data for an entity used in an encounter.
typedef struct {
    Encounterable* encounterable;

    uint8_t effect_duration;
    int8_t intelligence_effect;
    int8_t strength_effect;
    int8_t speed_effect;

    EffectiveStats effective_stats;
    bool is_defending;
} EncounterEntity;

typedef struct {
	uint16_t damage;
	uint8_t crit;
} Damage;

// The way speed works is that it's added to the counter, and when it reaches 255 you get a turn.
// Pretty much copied from 'In Stars And Time'.
extern uint8_t encounter_turn_counter_player;
extern uint8_t encounter_turn_counter_enemy;

extern int8_t encounter_enemy_animation[16];
extern uint8_t encounter_enemy_animation_index;
extern uint8_t encounter_text_scroll;

extern uint8_t encounter_animation_damage_numbers[5];
extern uint8_t encounter_animation_damage_crit;
extern uint8_t encounter_animation_damage_animation_index;

extern uint8_t encounter_held;
extern uint8_t encounter_just_pressed;
extern uint8_t encounter_just_released;

void encounter_enable_interrupts(void);
void encounter_disable_interrupts(void);

void encounter_draw_initial_ui(void);

void encounter_swap_button_color(uint8_t menu_button);
void encounter_swap_button_parity(void);

void encounter_button_move_left(uint8_t menu_button);
void encounter_button_move_right(uint8_t menu_button);

void encounter_draw_initial_bar_tiles(void);

void encounter_draw_player_health_bar(uint16_t health, uint16_t max_health);
void encounter_draw_enemy_health_bar(uint16_t health, uint16_t max_health);

void encounter_draw_player_turn_bar(uint8_t turn_counter, bool full);
void encounter_draw_enemy_turn_bar(uint8_t turn_counter, bool full);

EffectiveStats calculate_effective_stats(EncounterEntity* entity);
Damage calculate_damage(EncounterEntity* attacker, EncounterEntity* target, uint8_t attack);

void encounter_player_turn(EncounterEntity* player, EncounterEntity* enemy);
void encounter_enemy_turn(EncounterEntity* player, EncounterEntity* enemy);

#endif // _game_encounter_h_INCLUDE
