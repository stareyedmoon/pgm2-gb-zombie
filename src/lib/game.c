#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <gb/gb.h>

#include <game.h>

#include <engine.h>
#include <general.h>
#include <defines.h>

#include <entity.h>
#include <player.h>
#include <zombie.h>

#include <item.h>


#include <resource/encounter_ui.h>
#include <resource/font.h>


void game_main(void) {
	return;
}

static int8_t zombie_offset = 0;
static void lcd_int_handler(void) {
	if (LCD_LYC == 0) {
		LCD_SCX = zombie_offset;
		LCD_LYC = 60;
	}
	else if (LCD_LYC == 60) {
		LCD_SCX = 0;
		LCD_LYC = 127;
	}
	else if (LCD_LYC == 127) {
		LCD_SCX = 4;
		LCD_LYC = 144;
	}
	else if (LCD_LYC == 144) {
		LCD_SCX = 0;
		LCD_LYC = 0;
	}
}

#define DRAW_INACTIVE_BUTTON(index) do {\
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 0, 0x04); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 1, 0x05); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 2, 0x05); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 3, 0x05); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 4, 0x04); \
		\
		/* These are 1 further to the right because the bottom two tiles is 1 further to the right for alignment reasons. */ \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 1, 0x06); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 2, 0x10 + index*4); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 3, 0x12 + index*4); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 4, 0x07); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 5, 0x00); \
		\
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 1, 0x02); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 2, 0x11 + index*4); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 3, 0x13 + index*4); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 4, 0x03); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 5, 0x00); \
	} while(0)

#define DRAW_ACTIVE_BUTTON(index) do {\
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 0, 0x04); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 1, 0x0D); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 2, 0x0D); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 3, 0x0D); \
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + index*5 + 4, 0x04); \
		\
		/* These are 1 further to the right because the bottom two tiles is 1 further to the right for alignment reasons. */ \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 1, 0x0E); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 2, 0x20 + index*4); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 3, 0x22 + index*4); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 4, 0x0F); \
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + index*5 + 5, 0x00); \
		\
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 1, 0x0A); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 2, 0x21 + index*4); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 3, 0x23 + index*4); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 4, 0x0B); \
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + index*5 + 5, 0x00); \
	} while(0)

uint8_t calculate_damage(Encounterable* damager, Encounterable* target) {
	uint8_t damage = damager->strength;

	if (damager->strength >= weapon_item[damager->weapon].strength_requirement && damager->intelligence >= weapon_item[damager->weapon].intelligence_requirement) {
		damage += weapon_item[damager->weapon].damage;
	}

	if (armor_item[target->armor].blunt_protection >= damage) return 1;
	else damage -= armor_item[target->armor].blunt_protection;

	int8_t damage_delta = (rand8() & 3) - (rand8() & 3);
	if (-damage_delta >= damage) return 1;
	damage += damage_delta;

	return damage;
}

void game_encounter(Encounterable* player, Encounterable* enemy, uint8_t* enemy_sprite) {
	// All of this is temporary, but I don't have time, so for now it stays.
	for (uint16_t y = 0; y < 7; y += 1) {
		for (uint16_t x = 0; x < 32; x += 1) {
			set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x08);
			set_vram_byte(TILEMAP0 + (y+8)*BUFFER_WIDTH + x, 0x80);
		}
	}
	for (uint16_t x = 0; x < 32; x += 1) {
		set_vram_byte(TILEMAP0 + BUFFER_WIDTH*7 + x, 0x0C);
		set_vram_byte(TILEMAP0 + BUFFER_WIDTH*15 + x, 0x04);
	}
	for (uint16_t y = 0; y < 8; y += 1) {
		for (uint16_t x = 0; x < 6; x += 1) {
			set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x+7, 0x40 + (y*6 + x));
		}
	}
	
	set_vram_byte(TILEMAP0 + BUFFER_WIDTH*16 + 0, 0x00);
	set_vram_byte(TILEMAP0 + BUFFER_WIDTH*17 + 0, 0x00);

	
	for (uint16_t i = 0; i < 4; i += 1) {
		if (i == 0) DRAW_ACTIVE_BUTTON(i);
		else DRAW_INACTIVE_BUTTON(i);
	}
	
	decompress_sprite(TILEBLOCK1 + 0x0000, font_data);
	decompress_sprite(TILEBLOCK2 + 0x0000, encounter_ui_data);
	decompress_sprite(TILEBLOCK2 + 0x0400, enemy_sprite);
	
	
	// There's an interrupt here so that the bottom 2 tiles are offset by 4 pixels.
	// The reason for this is that this lets me save a couple tiles on the icons.
	// As a bonus, it also lets me do some shaking effects.
	disable_interrupts();
	
	LCD_SCY = 0;
	LCD_LYC = 127;
	
	CRITICAL {
		LCD_STAT &= 0x83;
		LCD_STAT |= 0x40;
		add_LCD(lcd_int_handler);
		add_LCD(nowait_int_handler);
	}
	
	enable_interrupts();
	set_interrupts(IE_REG | LCD_IFLAG);
	



	uint8_t zombie_shake_frame = 8;
	int8_t zombie_shake[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	uint8_t current_button = 0;
	uint8_t current_menu_selection = 0;
	bool changed_menu = true;

	// States:
	// 0 - Select button
	// 1 - Fight menu
	// 2 - Food menu
	// 3 - 
	// 4 - Zombies turn
	// 253 - You win
	// 254 - You lose
	// 255 - Return when animation is done.
	uint8_t state = 0;

	static const int8_t zombie_shake_hurt[8] = {0, -3, -5, -4, -3, -2, -1, 0};
	static const int8_t zombie_shake_hurt_you[8] = {0, 0, 0, 0, 0, 0, 10, 5};
	static const int8_t zombie_shake_run[8] = {0, 15, 30, 45, 59, 74, 89, 104};
	
	uint8_t prev_joy = 0;

	while (true) {
		uint8_t cur_joy = joypad();
		// Bitmaps of which buttons were just pressed and released.
		uint8_t just_pressed = cur_joy & ~prev_joy;
		uint8_t just_released = prev_joy & ~cur_joy;

		// Temporary HP display.
		// Don't know where to actually put it yet.
		char hp_disp[4];
		sprintf(hp_disp, "%u", player->health);
		engine_render_text(TILEMAP0, hp_disp, 0, 0, 3, 1, false, TEXTMODE_NOSCROLL);

		// Left and right change which menu we select
		if ((just_pressed & J_LEFT) && current_button > 0 && state == 0) {
			DRAW_INACTIVE_BUTTON(current_button);
			current_button -= 1;
			DRAW_ACTIVE_BUTTON(current_button);
			changed_menu = true;
		}
		if ((just_pressed & J_RIGHT) && current_button < 3 && state == 0) {
			DRAW_INACTIVE_BUTTON(current_button);
			current_button += 1;
			DRAW_ACTIVE_BUTTON(current_button);
			changed_menu = true;
		}

		// Up and down change what we select in a menu
		if ((just_pressed & J_UP) && current_menu_selection > 0) {
			if (state == 1) {
				set_vram_byte(TILEMAP0 + BUFFER_WIDTH*(11 + current_menu_selection), 0x80);
				current_menu_selection -= 1;
				set_vram_byte(TILEMAP0 + BUFFER_WIDTH*(11 + current_menu_selection), 0x9B);
			}
		}
		if ((just_pressed & J_DOWN)) {
			if (state == 1 && current_menu_selection < 3) {
				set_vram_byte(TILEMAP0 + BUFFER_WIDTH*(11 + current_menu_selection), 0x80);
				current_menu_selection += 1;
				set_vram_byte(TILEMAP0 + BUFFER_WIDTH*(11 + current_menu_selection), 0x9B);
			}
		}

		// When we have changed the menu we need to draw the one we are now in.
		if (changed_menu) {
			char menu_text[141];
			switch (current_button) {
				case 0: // Fight menu
				sprintf(menu_text, "%s\n%u/%u HP\n\n %s\n %s\n %s\n %s", "JON BIGRM", enemy->health, enemy->max_health, "ATTACK A", "ATTACK B", "ATTACK C", "ATTACK D");
				current_menu_selection = 0;
				
				//engine_render_text(TILEMAP0, "JON BIGRM\n???/??? HP\n\n-", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			case 1: // Item menu
				sprintf(menu_text, "don't get hurt.\n\nTO BE CLEAR, NO TIMETO IMPLEMENT.\nBTW THERE IS NO\nDIFFERENCE BETWEEN\nATTACKS YET.");
				current_menu_selection = 0;

				//engine_render_text(TILEMAP0, "ITEMS GO HERE.", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			case 2: // Info menu
				sprintf(menu_text, "%s\n--------------------%s\n--------------------VIT %s%u      INT %s%uSTR %s%u      SPD %s%u", 
					"JON BIGRM",
					"IT'S GOT REAL BIG\nARMS OR SOMETHING.",
					enemy->max_health < 100 ? (enemy->max_health < 10 ? "  " : " ") : "", enemy->max_health,
					enemy->intelligence < 100 ? (enemy->intelligence < 10 ? "  " : " ") : "", enemy->intelligence,
					enemy->strength < 100 ? (enemy->strength < 10 ? "  " : " ") : "", enemy->strength,
					enemy->speed < 100 ? (enemy->speed < 10 ? "  " : " ") : "", enemy->speed);

				//engine_render_text(TILEMAP0, "WHO THIS?", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			case 3: // Run away
				sprintf(menu_text, "I DON'T WANNA DEAL\nWITH THIS!");

				//engine_render_text(TILEMAP0, "RUN AWAY FROM THE\nFIGHT.", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			}

			engine_render_text(TILEMAP0, menu_text, 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);

			changed_menu = false;
		}

		// A goes forward.
		if (just_pressed & J_A) {
			if (state == 0) {
				if (current_button == 0) { state = 1; set_vram_byte(TILEMAP0 + BUFFER_WIDTH*(11 + current_menu_selection), 0x9B); }
				if (current_button == 1) state = 2;
				if (current_button == 3) { state = 255; memcpy(zombie_shake, zombie_shake_run, 8); zombie_shake_frame = 0; }
			}
			else if (state == 1) {
				uint8_t damage = calculate_damage(player, enemy);
				
				memcpy(zombie_shake, zombie_shake_hurt, 8);
				zombie_shake_frame = 0;
				
				if (damage >= enemy->health) state = 253;
				else state = 4;
				enemy->health -= damage;	
			}
		}
		// B for going Back.
		if (just_pressed & J_B) {
			if (state == 1) { state = 0; set_vram_byte(TILEMAP0 + BUFFER_WIDTH*(11 + current_menu_selection), 0x80); }
			if (state == 2) state = 0;
		}

		// We only let the zombie take a turn when the animation is done.
		if (state == 4 && zombie_shake_frame == 8) {
			uint8_t damage = calculate_damage(enemy, player);

			memcpy(zombie_shake, zombie_shake_hurt_you, 8);
			zombie_shake_frame = 0;

			if (damage >= player->health) state = 254;
			else { state = 0; changed_menu = true; }
			player->health -= damage;
		}

		if (state == 253) {
			engine_render_text(TILEMAP0, "YOU WIN :)", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
			state = 255;
		}
		if (state == 254) {
			engine_render_text(TILEMAP0, "YOU LOSE :(", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
			state = 255;
		}

		if (state == 255 && zombie_shake_frame == 8) {
			return;
		}

		// When `zombie_shake_frame` is less than 8, we are currently doing an animation.
		if (zombie_shake_frame < 8) {
			zombie_offset = zombie_shake[zombie_shake_frame];
			zombie_shake_frame += 1;
		}
		else {
			zombie_offset = 0;
		}

		prev_joy = cur_joy;
		vsync();
		rand8();
	}
}
