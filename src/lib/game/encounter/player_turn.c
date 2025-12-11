#include <stdint.h>
#include <stdbool.h>

#include <gb/gb.h>

#include <game/encounter.h>

#include <defines.h>
#include <general.h>

static void draw_player_attack_menu(EncounterEntity* player, EncounterEntity* enemy) {}
static void draw_player_item_menu(EncounterEntity* player, EncounterEntity* enemy) {}
static void draw_player_info_menu(EncounterEntity* player, EncounterEntity* enemy) {}
static void draw_player_run_menu(EncounterEntity* player, EncounterEntity* enemy) {}


static void player_turn_attack(EncounterEntity* player, EncounterEntity* enemy) {
	static const int8_t damage_shake[16] = {25, 49, 55, 50, 38, 22, 7, -6, -16, -20, -20, -16, -9, -2, -2, 0};
	static const uint8_t shake_scale_linexp_lut[17] = {
		0, 1, 3, 6, 12, 18, 28, 43, 65, 99, 151, 215, 252, 255, 255, 255, 255
	};

	Damage damage = calculate_damage(player, enemy);

	uint8_t shake_scale = 0;

	if (damage.damage > 0) {
		uint8_t damage_exp = log2l(damage.damage);
		shake_scale = linear_interp(
			shake_scale_linexp_lut[damage_exp],
			shake_scale_linexp_lut[damage_exp + 1],
			0xFF & (damage_exp < 8
				? damage.damage << (8 - damage_exp)
				: damage.damage >> (damage_exp - 8))
		);
	}

	for (uint8_t i = 0; i < 16; i += 1) {
		int16_t scaled_shake = damage_shake[i];

		scaled_shake *= shake_scale;
		scaled_shake /= 128;
		if (scaled_shake > 104) scaled_shake = 104;
		if (scaled_shake < -104) scaled_shake = -104;

		encounter_enemy_animation[i] = scaled_shake;
	}

	enemy->encounterable->health -= MIN(enemy->encounterable->health, damage.damage);

	encounter_enemy_animation_index = 0;
	
	// TODO - Slash animation
	// TODO - Show damage
}
static void player_turn_items(EncounterEntity* player, EncounterEntity* enemy) {

}

void encounter_player_turn(EncounterEntity* player, EncounterEntity* enemy) {
    encounter_swap_button_color(0);
    // TODO - Find a neater way to do this.
    set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 0, 0x18);
    set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 1, 0x19);
    set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x1B);
    set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x1C);
    set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x1D);
    
    uint8_t menu_button = 0;
    bool menu_changed = true;

    uint8_t prev_joy = joypad();

    while (true) {
		uint8_t cur_joy = joypad();
		// Bitmaps of which buttons were just pressed and released.
		uint8_t just_pressed = cur_joy & ~prev_joy; 
		uint8_t just_released = prev_joy & ~cur_joy;


        if (just_pressed & J_LEFT && menu_button > 0) {
            menu_button -= 1;
            menu_changed = true;
            encounter_button_move_left(menu_button);
        }
        if (just_pressed & J_RIGHT && menu_button < 3) {
            menu_button += 1;
            menu_changed = true;
            encounter_button_move_right(menu_button);
        }
        if (menu_changed) {
            encounter_text_scroll = 0;

            switch (menu_button) {
            case 0: draw_player_attack_menu(player, enemy); break;
            case 1: draw_player_item_menu(player, enemy); break;
            case 2: draw_player_info_menu(player, enemy); break;
            case 3: draw_player_run_menu(player, enemy); break;
            }

            menu_changed = false;
        }

        if (just_pressed & J_A) {
            // Attack
            if (menu_button == 0) {
                player_turn_attack(player, enemy);
                encounter_draw_enemy_health_bar(enemy->encounterable->health, enemy->encounterable->max_health);
                encounter_turn_counter_player -= 128;
            }
            // Item
            else if (menu_button == 1) {
                player_turn_items(player, enemy);
                encounter_turn_counter_player -= 32;
            }
            // TODO - Implement running away

            break;
        }


        prev_joy = cur_joy;

        vsync();
    }

    encounter_swap_button_color(menu_button);
    // TODO - Put this code somewhere else or figure out a way to remove it entirely.
    switch (menu_button) {
    case 0:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 0, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 1, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x15);
        break;
    case 1:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 2, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 2, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 2, 0x15);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 3, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 4, 0x12);
        encounter_swap_button_parity();
        break;
    case 2:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 15, 0x10);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 16, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 17, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 17, 0x15);
        break;
    case 3:
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 17, 0x13);
        set_vram_byte(TILEMAP0 + 30*BUFFER_WIDTH + 17, 0x14);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 17, 0x15);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 18, 0x11);
        set_vram_byte(TILEMAP0 + 29*BUFFER_WIDTH + 19, 0x12);
        encounter_swap_button_parity();
        break;
    }
}
