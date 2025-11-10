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



static int8_t encounter_zombie_scroll = 0;
static uint8_t encounter_menu_button = 0;


static void lcd_int_handler(void) {
	disable_interrupts();
	if (LCD_LYC == 0) {
		LCD_SCX = encounter_zombie_scroll;

        LCD_LYC = 60;
	}
	else if (LCD_LYC == 60) {
		LCD_SCX = 0;

        LCD_LYC = 0;
	}
	enable_interrupts();
}

static void draw_buttons(void) {

	// Temporary
	static const uint8_t tile_lut[] = {
		0x0D, 0x0E, 0x0A, 0x06, 0x07, 0x05, 0x06, 0x02, 0x06, 0x07,
		0x10, 0x11, 0x0B, 0x14, 0x15, 0x18, 0x19, 0x03, 0x1C, 0x1D,
		0x12, 0x13, 0x0C, 0x16, 0x17, 0x1A, 0x1B, 0x04, 0x1E, 0x1F,
	};

	for (uint16_t y = 15; y < 18; y += 1) {
		for (uint16_t x = 0; x < 10; x += 1) {
			uint8_t i = x + (y - 15) * 10;

			set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, tile_lut[i]);
		}
	}
}

static void swap_button_color(void) {
	uint16_t offset_start = (uint16_t)(0x10 + encounter_menu_button * 4) * 16;

	for (uint16_t i = 0; i < 128; i += 2) {
		uint8_t a = get_vram_byte(TILEBLOCK2 + offset_start + i);
		uint8_t b = get_vram_byte(TILEBLOCK2 + offset_start + i + 1);

		set_vram_byte(TILEBLOCK2 + offset_start + i, b);
		set_vram_byte(TILEBLOCK2 + offset_start + i + 1, a);
	}
}

void game_encounter(Encounterable* player, Encounterable* enemy, uint8_t* enemy_sprite) {
	for (uint16_t y = 0; y < 7; y += 1) {
		for (uint16_t x = 0; x < 32; x += 1) {
			set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x08);
			set_vram_byte(TILEMAP0 + (y+8)*BUFFER_WIDTH + x, 0x50);
		}
	}
	for (uint16_t x = 0; x < 32; x += 1) {
		set_vram_byte(TILEMAP0 + 7*BUFFER_WIDTH + x, 0x09);
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + x, 0x01);
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + x, 0x00);
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + x, 0x00);
	}
    for (uint16_t y = 0; y < 8; y += 1) {
        for (uint16_t x = 7; x < 13; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x20 + y*6 + x - 7);
        }
    }

    draw_buttons();

	decompress_sprite(TILEBLOCK2 + 0x0000 /* 00-1F */, encounter_ui_data);
	decompress_sprite(TILEBLOCK2 + 0x0200 /* 20-4F */, enemy_sprite);
	decompress_sprite(TILEBLOCK2 + 0x0500 /* 50-7F */, font_data);

	encounter_menu_button = 0;

	disable_interrupts();
	
	LCD_SCY = 0;
	LCD_LYC = 0;
	
	CRITICAL {
		LCD_STAT &= 0x83;
		LCD_STAT |= 0x40;
		add_LCD(lcd_int_handler);
		add_LCD(nowait_int_handler);
	}
	
	enable_interrupts();
	set_interrupts(IE_REG | LCD_IFLAG);

	uint8_t prev_joy = 0;

	while (true) {
		uint8_t cur_joy = joypad();
		// Bitmaps of which buttons were just pressed and released.
		uint8_t just_pressed = cur_joy & ~prev_joy;
		uint8_t just_released = prev_joy & ~cur_joy;

		if (just_pressed & J_LEFT && encounter_menu_button > 0) {
			encounter_menu_button -= 1;
			swap_button_color();
            for (uint8_t i = 0xA0; i < 0xD0; i += 1) {
                set_vram_byte(TILEBLOCK2 + i, flip_byte(get_vram_byte(TILEBLOCK2 + i)));
            }

            switch (encounter_menu_button) {
            case 0:
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 0, 0x0D);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 1, 0x0E);
                //set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 2, 0x12);
                //set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 2, 0x13);
                //set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 2, 0x14);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 3, 0x06);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 4, 0x07);
                break;
            case 1:
                //set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 2, 0x1A);
                //set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 2, 0x1B);
                //set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 2, 0x1C);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 2, 0x0A);
                set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 2, 0x0B);
                set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 2, 0x0C);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 3, 0x0E);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 4, 0x0F);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 5, 0x05);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 6, 0x06);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 7, 0x02);
                set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 7, 0x03);
                set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 7, 0x04);
                break;
            case 2:
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 5, 0x0D);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 6, 0x0E);
                //set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 7, 0x12);
                //set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 7, 0x13);
                //set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 7, 0x14);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 8, 0x06);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 9, 0x07);
                break;
            }
		}
		if (just_pressed & J_RIGHT && encounter_menu_button < 3) {
			swap_button_color();
            for (uint8_t i = 0xA0; i < 0xD0; i += 1) {
                set_vram_byte(TILEBLOCK2 + i, flip_byte(get_vram_byte(TILEBLOCK2 + i)));
            }
			encounter_menu_button += 1;

            switch (encounter_menu_button) {
            case 1:
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 0, 0x05);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 1, 0x06);
                //set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 2, 0x1A);
                //set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 2, 0x1B);
                //set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 2, 0x1C);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 3, 0x0E);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 4, 0x0F);
                break;
            case 2:
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 2, 0x02);
                set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 2, 0x03);
                set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 2, 0x04);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 3, 0x06);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 4, 0x07);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 5, 0x0D);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 6, 0x0E);
                //set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 7, 0x12);
                //set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 7, 0x13);
                //set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 7, 0x14);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 7, 0x0A);
                set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 7, 0x0B);
                set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 7, 0x0C);
                break;
            case 3:
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 5, 0x05);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 6, 0x06);
                //set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 7, 0x1A);
                //set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 7, 0x1B);
                //set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + 7, 0x1C);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 8, 0x0E);
                set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + 9, 0x0F);
                break;
            }
        }

		prev_joy = cur_joy;
		vsync();
		rand8();
	}
}
