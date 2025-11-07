#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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
	bool changed_menu = true;

	// States:
	// 0 - Select button
	// 1 - Fight menu
	// 2 - Food menu
	// 3 - Run away
	// 4 - Zombies turn
	uint8_t state = 0;
	
	static const int8_t zombie_shake_hurt[8] = {0, -3, -5, -4, -3, -2, -1, 0};
	static const int8_t zombie_shake_run[8] = {0, 15, 30, 45, 59, 74, 89, 104};
	
	uint8_t prev_joy = 0;

	while (true) {
		uint8_t cur_joy = joypad();
		// Bitmaps of which buttons were just pressed and released.
		uint8_t just_pressed = cur_joy & ~prev_joy;
		uint8_t just_released = prev_joy & ~cur_joy;

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

		// When we have changed the menu we need to draw the one we are now in.
		if (changed_menu) {
			switch (current_button) {
			case 0: // Fight menu
				engine_render_text(TILEMAP0, "JON BIGRM\n???/??? HP\n\n-", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			case 1: // Item menu
				engine_render_text(TILEMAP0, "ITEMS GO HERE.", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			case 2: // Info menu
				engine_render_text(TILEMAP0, "WHO THIS?", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			case 3: // Run away
				engine_render_text(TILEMAP0, "RUN AWAY FROM THE\nFIGHT.", 0, 8, 20, 7, false, TEXTMODE_NOSCROLL);
				break;
			}

			changed_menu = false;
		}

		// A goes forward.
		if (just_pressed & J_A) {
			if (state == 0) {
				if (current_button == 0) state = 1;
				if (current_button == 1) state = 2;
				if (current_button == 3) { state = 3; memcpy(zombie_shake, zombie_shake_run, 8); zombie_shake_frame = 0; }
			}
		}
		// B for going Back.
		if (just_pressed & J_B) {
			if (state == 1) state = 0;
			if (state == 2) state = 0;
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
	}
}
