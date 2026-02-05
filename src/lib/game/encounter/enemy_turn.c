#include <stdint.h>

#include <game/encounter.h>

#include <defines.h>
#include <general.h>

#include <engine.h>

#include <item.h>

void encounter_enemy_turn(EncounterEntity* player, EncounterEntity* enemy) {
    enemy->is_defending = false;

    for (uint8_t i = 0; i < 4; i += 1) {
        if (enemy->attack_cooldown[i] > 0) enemy->attack_cooldown[i] -= 1;
    }

    WeaponItem* weapon = &weapon_item[enemy->encounterable->weapon];

    uint8_t attack_choice = 255;
    int8_t choice_weight = -128;
    for (uint8_t i = 0; i < 4; i += 1) {
        int8_t current_weight = 0;

        if (weapon->attacks[i].type == DAMAGE_TYPE_DEFEND) {
            uint8_t time_until_player_turn = -encounter_turn_counter_player / player->effective_stats.speed;
            uint8_t time_until_enemy_turn = -encounter_turn_counter_enemy / enemy->effective_stats.speed;

            if (time_until_enemy_turn > time_until_player_turn) {
                choice_weight = MIN(127, weapon_item[enemy->encounterable->weapon].damage + enemy->encounterable->max_health - enemy->encounterable->health);
            }
            else {
                // Not -128 because if there are no other options it would still be better than doing nothing.
                current_weight = -127;
            }
        }
        else {
            current_weight = MIN(calculate_expected_damage(enemy, player, i), 127);
        }

        if (enemy->attack_cooldown[i] != 0) current_weight = -128;

        if (current_weight > choice_weight) {
            attack_choice = i;
            choice_weight = current_weight;
        }
    }

    char* output[121];
    if (attack_choice == 255) {
        encounter_turn_counter_enemy -= 128;

        format(output, "%s\nFAILED DOING ANYTHNG",
            enemy->encounterable->name
        );
    }
    else {
        encounter_turn_counter_enemy -= weapon->attacks[attack_choice].time_cost;
        enemy->attack_cooldown[attack_choice] = weapon->attacks[attack_choice].cooldown;

        
        if (weapon->attacks[attack_choice].type != DAMAGE_TYPE_DEFEND) {
            Damage damage = calculate_damage(enemy, player, attack_choice);

            player->encounterable->health -= MIN(player->encounterable->health, damage.damage);

            format(output, "%s USED\n%s\nIT DOES %w DAMAGE.",
                enemy->encounterable->name,
                weapon->attacks[attack_choice].name,
                damage.damage
            );
        }
        else {
            enemy->is_defending = true;

            format(output, "%s USED\n%s",
                enemy->encounterable->name,
                weapon->attacks[attack_choice].name
            );
        }
    }

    e_text_speed = 1;
    engine_render_text(TILEMAP0, output, 0, 10, 20, 6, false, TEXTMODE_NOSCROLL);
    e_text_speed = 0;

    encounter_draw_player_health_bar(player->encounterable->health, player->encounterable->max_health);

    set_vram_byte(TILEMAP0 + 0x1F3, 0xB0);

    while (!(encounter_just_pressed & J_A)) vsync();
    
    engine_render_text(TILEMAP0, "", 0, 10, 20, 6, false, TEXTMODE_NOSCROLL);
}
