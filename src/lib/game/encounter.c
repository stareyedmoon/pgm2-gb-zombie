#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <gb/gb.h>

#include <game.h>
#include <game/encounter.h>

#include <engine.h>
#include <general.h>
#include <defines.h>

#include <entity.h>
#include <item.h>

#include <resource/encounter_sprites.h>
#include <resource/encounter_ui.h>
#include <resource/font.h>

uint8_t encounter_turn_counter_player = 0;
uint8_t encounter_turn_counter_enemy = 0;

void game_encounter(Encounterable* player, Encounterable* enemy, uint8_t* enemy_sprite) {
    EncounterEntity encounter_player = {player, 0, 0, 0, 0, {0, 0, 0, false}, false};
    EncounterEntity encounter_enemy = {enemy, 0, 0, 0, 0, {0, 0, 0, false}, false};

    encounter_draw_initial_ui();

    decompress_sprite(TILEBLOCK0 + 0x0000 /* 00-?? (sprites) */, encounter_sprites_data);
    decompress_sprite(TILEBLOCK2 + 0x0000 /* 00-2F */, encounter_ui_data);
    encounter_draw_initial_bar_tiles(); /* 30-3F */
	decompress_sprite(TILEBLOCK2 + 0x0500 /* 50-7F */, enemy_sprite);
	decompress_sprite(TILEBLOCK1 + 0x0000 /* 80-AF */, font_data);


    encounter_draw_player_health_bar(player->health, player->max_health);
    encounter_draw_enemy_health_bar(enemy->health, enemy->max_health);
    encounter_draw_player_turn_bar(0, false);
    encounter_draw_enemy_turn_bar(0, false);

    encounter_enable_interrupts();

    encounter_turn_counter_player = 0;
    encounter_turn_counter_enemy = 0;

	while (true) {
        encounter_player.effective_stats = calculate_effective_stats(&encounter_player);
        encounter_enemy.effective_stats = calculate_effective_stats(&encounter_enemy);

        encounter_turn_counter_player += encounter_player.effective_stats.speed;
        encounter_turn_counter_enemy += encounter_enemy.effective_stats.speed;

        // When an integer overflows on an add, the result is always smaller than the value added.
        if (encounter_turn_counter_player < encounter_player.effective_stats.speed) {
            encounter_draw_player_turn_bar(0, true);
            encounter_player_turn(&encounter_player, &encounter_enemy);
        }
        encounter_draw_player_turn_bar(encounter_turn_counter_player, false);
        
	if (encounter_turn_counter_enemy < encounter_enemy.effective_stats.speed) {
            encounter_draw_enemy_turn_bar(0, true);
            encounter_enemy_turn(&encounter_player, &encounter_enemy);
        }
        encounter_draw_enemy_turn_bar(encounter_turn_counter_enemy, false);

		vsync();
	}

    encounter_disable_interrupts();
}
