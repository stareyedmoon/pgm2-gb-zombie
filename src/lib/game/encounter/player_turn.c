#include <stdint.h>
#include <stdbool.h>

#include <gb/gb.h>

#include <game/encounter.h>

#include <defines.h>
#include <general.h>

#include <engine.h>

#include <item.h>

// Possible mechanic for running away
//   The player has to survive some number of turns without agency while the enemy attacks them

static void draw_player_attack_menu(EncounterEntity* player, EncounterEntity* enemy) {
    char* const menu[121];

    WeaponItem* weapon = &weapon_item[player->encounterable->weapon];

    format(menu, "%s\n\n %s\n %s\n %s\n %s",
        enemy->encounterable->name,
        weapon->attacks[0].name,
        weapon->attacks[1].name,
        weapon->attacks[2].name,
        weapon->attacks[3].name
    );

    engine_render_text(TILEMAP0, menu, 0, 10, 20, 6, false, TEXTMODE_NOSCROLL);
}
static void draw_player_item_menu(EncounterEntity* player, EncounterEntity* enemy) {
    engine_render_text(TILEMAP0, "ITEMS!", 0, 10, 20, 6, false, TEXTMODE_NOSCROLL);
}
static void draw_player_info_menu(EncounterEntity* player, EncounterEntity* enemy) {
    char* const info[121];

    format(info, "%s\n HP %W  LV  %WVIT %W  INT %WSTR %W  SPD %WWPN %s\nARM %s",
        enemy->encounterable->name,
        enemy->encounterable->health, enemy->encounterable->level,
        enemy->encounterable->max_health, enemy->encounterable->intelligence,
        enemy->encounterable->strength, enemy->encounterable->speed,
        weapon_item[enemy->encounterable->weapon].name,
        armor_item[enemy->encounterable->armor].name
    );

    engine_render_text(TILEMAP0, info, 0, 10, 20, 8, false, TEXTMODE_NOSCROLL);
}
static void draw_player_run_menu(EncounterEntity* player, EncounterEntity* enemy) {
    engine_render_text(TILEMAP0, "SCRET JOESTAR FAMILYTECHNIQUE", 0, 10, 20, 6, false, TEXTMODE_NOSCROLL);
}

static void set_enemy_shake(uint16_t damage) {
	// The base animation used for the enemy shake.
	// This is scaled according to the damage so it moves more if it hurts more.
	static const int8_t damage_shake[16] = {25, 49, 55, 50, 38, 22, 7, -6, -16, -20, -18, -16, -9, -2, -1, 0};
	
	// The scale of the shake. A value of 128 is unscaled.
	// The values of this LUT are for the power-of-2 bounderies, and are linearly interpolated between.
	// As an example, for 0x00C3 (95) damage, it would interpolate between a scale of 65 and 99.
	// For the linear interpolation the remaining bits after the MSB are used.
	// This means that for this example a value of 0x86 ((0xC3 << 1) & 0xFF) is used.
	// The result is ((65*0x79)+(99*0x86))/255, giving 82 for this example.
	static const uint8_t shake_scale_linexp_lut[17] = {
		0, 1, 3, 6, 12, 18, 28, 43, 65, 99, 151, 215, 252, 255, 255, 255, 255
	};
	
	uint8_t shake_scale = 0;

	if (damage > 0) {
		uint8_t damage_exp = log2l(damage);

		shake_scale = linear_interp(
			shake_scale_linexp_lut[damage_exp],
			shake_scale_linexp_lut[damage_exp + 1],
		
			// This gets the remaining bits after the MSB and shifts it to take 8 bits
			0xFF & (damage_exp <= 8
				? damage << (9 - damage_exp)
				: damage >> (damage_exp - 9))
		);
	}

	for (uint8_t i = 0; i < 16; i += 1) {
		int16_t scaled_shake = damage_shake[i];

		scaled_shake *= shake_scale;
		scaled_shake /= 128;
		// A value of 104 or above is off-screen, so this bounds check is to make sure it doesn't wrap around.
		if (scaled_shake > 104) scaled_shake = 104;
		if (scaled_shake < -104) scaled_shake = -104;

		encounter_enemy_animation[i] = scaled_shake;
	}
}
static void set_damage_animation(uint16_t damage, uint8_t crit) {
	for (uint8_t i = 0; i < 5; i += 1) {
		encounter_animation_damage_numbers[4-i] = damage % 10;
		damage /= 10;
	}
    for (uint8_t i = 0; i < 4; i += 1) {
        if (encounter_animation_damage_numbers[i]) break;
        encounter_animation_damage_numbers[i] = 10;
    }
	encounter_animation_damage_crit = crit;
}

static void player_turn_attack(EncounterEntity* player, EncounterEntity* enemy) {
    static uint8_t selected_attack = 0;
    
    encounter_just_pressed &= ~J_A;

    set_vram_byte(TILEMAP0 + 0x180 + 0x20*selected_attack, 0x9B);

    while (true) {
        if (encounter_just_pressed & J_UP && selected_attack > 0) {
            set_vram_byte(TILEMAP0 + 0x180 + 0x20*selected_attack, 0x80);
            selected_attack -= 1;
            set_vram_byte(TILEMAP0 + 0x180 + 0x20*selected_attack, 0x9B);
        }
        if (encounter_just_pressed & J_DOWN && selected_attack < 3) {
            set_vram_byte(TILEMAP0 + 0x180 + 0x20*selected_attack, 0x80);
            selected_attack += 1;
            set_vram_byte(TILEMAP0 + 0x180 + 0x20*selected_attack, 0x9B);
        }
        
        if (encounter_just_pressed & J_A) break;

        vsync();
    }

    encounter_turn_counter_player -= weapon_item[player->encounterable->weapon].attacks[selected_attack].time_cost;

	Damage damage = calculate_damage(player, enemy, selected_attack);
	
	enemy->encounterable->health -= MIN(enemy->encounterable->health, damage.damage);

	set_enemy_shake(damage.damage);
	set_damage_animation(damage.damage, damage.crit);
	encounter_enemy_animation_index = 0;
	encounter_animation_damage_animation_index = 0;
	
	// TODO - Slash animation
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

    while (true) {
        if (encounter_just_pressed & J_LEFT && menu_button > 0) {
            menu_button -= 1;
            menu_changed = true;
            encounter_button_move_left(menu_button);
        }
        if (encounter_just_pressed & J_RIGHT && menu_button < 3) {
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

        if (encounter_just_pressed & J_A) {
            // Attack
            if (menu_button == 0) {
                player_turn_attack(player, enemy);
                encounter_draw_enemy_health_bar(enemy->encounterable->health, enemy->encounterable->max_health);
            }
            // Item
            else if (menu_button == 1) {
                player_turn_items(player, enemy);
                encounter_turn_counter_player -= 32;
            }
            // TODO - Implement running away

            break;
        }

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
