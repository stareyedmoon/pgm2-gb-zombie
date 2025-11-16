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

// The way speed works is that it's added to the counter, and when it reaches 255 you get a turn.
// Pretty much copied from 'In Stars And Time'.
static uint8_t encounter_turn_counter_player = 0;
static uint8_t encounter_turn_counter_enemy = 0;


static int8_t encounter_zombie_scroll = 0;
static uint8_t encounter_menu_button = 0;


static void lcd_int_handler(void) {
	disable_interrupts();
	if (LCD_LYC == 0) {
		LCD_SCX = encounter_zombie_scroll;
        LCD_SCY = 0;

        LCD_LYC = 60;
	}
	else if (LCD_LYC == 60) {
		LCD_SCX = 0;
        LCD_SCY = 3;

        LCD_LYC = 68;
	}
    else if (LCD_LYC == 68) {
        LCD_SCX = 0;
        LCD_SCY = 8;

        LCD_LYC = 124;
    }
    else if (LCD_LYC == 124) {
        LCD_SCX = 0;
        LCD_SCY = 17;

        LCD_LYC = 0;
    }
	enable_interrupts();
}

static void enable_lcd_interrupt(void) {
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
}
static void disable_lcd_interrupt(void) {

}

static void draw_buttons(void) {

	// Temporary
	static const uint8_t tile_lut[] = {
		0x0D, 0x0E, 0x0A, 0x06, 0x07, 0x05, 0x06, 0x02, 0x06, 0x07,
		0x10, 0x11, 0x0B, 0x14, 0x15, 0x18, 0x19, 0x03, 0x1C, 0x1D,
		0x12, 0x13, 0x0C, 0x16, 0x17, 0x1A, 0x1B, 0x04, 0x1E, 0x1F,
	};

	for (uint16_t y = 7; y < 18; y += 1) {
		for (uint16_t x = 0; x < 10; x += 1) {
			uint8_t i = x + (y - 7) * 10;

			set_vram_byte(TILEMAP1 + y*BUFFER_WIDTH + x, tile_lut[i]);
		}
	}
}
static void draw_initial_ui(void) {
    //LCD_CTRL |= 0x20; // Enable window layer
    //LCD_WX = 7;
    //LCD_WY = 64;

    for (uint16_t y = 0; y < 9; y += 1) {
        for (uint16_t x = 0; x < BUFFER_WIDTH; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x00);
        }
    }
    for (uint16_t y = 10; y < 16; y += 1) {
        for (uint16_t x = 0; x < BUFFER_WIDTH; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x80);
        }
    }
    for (uint16_t y = 17; y < BUFFER_HEIGHT; y += 1) {
        for (uint16_t x = 0; x < BUFFER_HEIGHT; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x08);
        }
    }

    for (uint16_t x = 0; x < 32; x += 1) {
        set_vram_byte(TILEMAP0 + 9*BUFFER_WIDTH + x, 0x02);
        set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + x, 0x0A);
    }
    for (uint16_t x = 0; x < 8; x += 1) {
        set_vram_byte(TILEMAP0 + 8*BUFFER_WIDTH + x + 6, 0x30 + x);
        set_vram_byte(TILEMAP0 + 19*BUFFER_WIDTH + x + 6, 0x38 + x);
    }

    set_vram_byte(TILEMAP0 + 8*BUFFER_WIDTH + 5, 0x1E);
    set_vram_byte(TILEMAP0 + 8*BUFFER_WIDTH + 14, 0x1F);
    set_vram_byte(TILEMAP0 + 19*BUFFER_WIDTH + 5, 0x16);
    set_vram_byte(TILEMAP0 + 19*BUFFER_WIDTH + 14, 0x17);

    set_vram_byte(TILEMAP0 + 9*BUFFER_WIDTH + 0, 0x01);
    set_vram_byte(TILEMAP0 + 9*BUFFER_WIDTH + 19, 0x03);
    set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 0, 0x09);
    set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + 19, 0x0B);

    for (uint16_t y = 0; y < 8; y += 1) {
        for (uint16_t x = 0; x < 6; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x + 7, 0x50 + y*6 + x);
        }
    }

    //draw_buttons();
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

static void set_player_health_bar(uint8_t health, uint8_t max_health) {}
static void set_enemy_health_bar(uint8_t health, uint8_t max_health) {}

static void set_player_turn_bar(bool full) {
    if (full) {
        for (uint8_t i = 0; i < 8; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 9, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 11, 0x00);
        }
    }
    else {
        uint8_t full_bars = encounter_turn_counter_player / 32;
        uint8_t partial = (0xFF) >> ((encounter_turn_counter_player / 4) % 8);

        for (uint8_t i = 0; i < full_bars; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 9, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 11, 0x00);
        }
        set_vram_byte(TILEBLOCK2 + 0x0380 + full_bars*16 + 9, partial);
        set_vram_byte(TILEBLOCK2 + 0x0380 + full_bars*16 + 11, partial);
        for (uint8_t i = full_bars + 1; i < 8; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 9, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 11, 0xFF);
        }
    }
}
static void set_enemy_turn_bar(bool full) {
    if (full) {
        for (uint8_t i = 0; i < 8; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 9, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 11, 0x00);
        }
    }
    else {
        uint8_t full_bars = encounter_turn_counter_enemy / 32;
        uint8_t partial = (0xFF) >> ((encounter_turn_counter_enemy / 4) % 8);

        for (uint8_t i = 0; i < full_bars; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 9, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 11, 0x00);
        }
        set_vram_byte(TILEBLOCK2 + 0x0300 + full_bars*16 + 9, partial);
        set_vram_byte(TILEBLOCK2 + 0x0300 + full_bars*16 + 11, partial);
        for (uint8_t i = full_bars + 1; i < 8; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 9, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 11, 0xFF);
        }
    }
}

static void player_turn(Encounterable* player, Encounterable* enemy) {
    encounter_turn_counter_player -= 128;

    for (uint8_t i = 0; i < 10; i += 1) vsync();
}
static void enemy_turn(Encounterable* player, Encounterable* enemy) {
    encounter_turn_counter_enemy -= 128;

    for (uint8_t i = 0; i < 10; i += 1) vsync();
}

void game_encounter(Encounterable* player, Encounterable* enemy, uint8_t* enemy_sprite) {
    draw_initial_ui();

    decompress_sprite(TILEBLOCK2 + 0x0000 /* 00-2F */, encounter_ui_data);
    /* 30-3F reserved for health and turn bars */
    for (uint8_t i = 0; i < 8; i += 1) {
        for (uint8_t j = 0; j < 8; j += 3) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + j*2 + 0, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + j*2 + 1, 0xFF);

            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + j*2 + 0, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + j*2 + 1, 0xFF);
        }

        for (uint8_t j = 1; j < 3; j += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + j*2 + 0, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + j*2 + 1, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + j*2 + 6, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + j*2 + 7, 0xFF);

            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + j*2 + 0, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + j*2 + 1, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + j*2 + 6, 0xFF);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + j*2 + 7, 0xFF);
        }

        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 14, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 15, 0x00);

        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 14, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 15, 0xFF);
    }
	decompress_sprite(TILEBLOCK2 + 0x0500 /* 50-7F */, enemy_sprite);
	decompress_sprite(TILEBLOCK1 + 0x0000 /* 80-AF */, font_data);
    set_player_health_bar(player->health, player->max_health);
    set_enemy_health_bar(enemy->health, enemy->max_health);
    set_player_turn_bar(false);
    set_enemy_turn_bar(false);

    enable_lcd_interrupt();


    encounter_turn_counter_player = 0;
    encounter_turn_counter_enemy = 0;
	encounter_menu_button = 0;

	uint8_t prev_joy = 0;

    uint8_t player_effective_speed = player->speed;
    uint8_t enemy_effective_speed = enemy->speed;

	while (true) {
		uint8_t cur_joy = joypad();
		// Bitmaps of which buttons were just pressed and released.
		uint8_t just_pressed = cur_joy & ~prev_joy; 
		uint8_t just_released = prev_joy & ~cur_joy;

        encounter_turn_counter_player += player_effective_speed;
        encounter_turn_counter_enemy += enemy_effective_speed;

        // When an integer overflows on an add, the result is always smaller than the value added.
        if (encounter_turn_counter_player < player_effective_speed) {
            set_player_turn_bar(true);
            player_turn(player, enemy);
        }
        set_player_turn_bar(false);
        if (encounter_turn_counter_enemy < enemy_effective_speed) {
            set_enemy_turn_bar(true);
            enemy_turn(player, enemy);
        }
        set_enemy_turn_bar(false);

		if (just_pressed & J_LEFT && encounter_menu_button > 0) {
			encounter_menu_button -= 1;
		}
		if (just_pressed & J_RIGHT && encounter_menu_button < 3) {
			encounter_menu_button += 1;
        }

		prev_joy = cur_joy;
		vsync();
		rand8();
	}
}
