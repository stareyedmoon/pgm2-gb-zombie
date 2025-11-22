#include <stdint.h>

#include <game/encounter.h>

void encounter_enemy_turn(EncounterEntity* player, EncounterEntity* enemy) {
    encounter_turn_counter_enemy -= 128;

    for (uint8_t i = 0; i < 10; i += 1) vsync();
}
