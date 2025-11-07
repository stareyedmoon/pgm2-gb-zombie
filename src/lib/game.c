#include <stdint.h>
#include <stdbool.h>

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

void lcd_int_handler(void) {
	if (LCD_LYC == 127) {
		LCD_SCX = 4;
		LCD_LYC = 144;
	}
	else if (LCD_LYC == 144) {
		LCD_SCX = 0;
		LCD_LYC = 127;
	}
}
void game_encounter(Encounterable* player, Encounterable* enemy, uint8_t* enemy_sprite) {
	// All of this is temporary, but I don't have time, so for now it stays.
	for (uint16_t y = 0; y < 7; y += 1) {
		for (uint16_t x = 0; x < 20; x += 1) {
			set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x08);
			set_vram_byte(TILEMAP0 + (y+8)*BUFFER_WIDTH + x, 0x80);
		}
	}
	for (uint16_t x = 0; x < 20; x += 1) {
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
	
	uint8_t current_button = 0;
	
	for (uint16_t i = 0; i < 4; i += 1) {
		uint8_t ioff = i * 5;
		bool select = current_button == i;
		uint8_t icon = i * 4;
		
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + ioff + 0, 0x04);
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + ioff + 1, 0x05 + (select?8:0));
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + ioff + 2, 0x05 + (select?8:0));
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + ioff + 3, 0x05 + (select?8:0));
		set_vram_byte(TILEMAP0 + 15*BUFFER_WIDTH + ioff + 4, 0x04);
		
		// These are 1 further to the right because the bottom two tiles is 1 further to the right for alignment reasons.
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + ioff + 1, 0x06 + (select?8:0));
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + ioff + 2, 0x10 + (select?16:0) + icon);
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + ioff + 3, 0x12 + (select?16:0) + icon);
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + ioff + 4, 0x07 + (select?8:0));
		set_vram_byte(TILEMAP0 + 16*BUFFER_WIDTH + ioff + 5, 0x00);

		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + ioff + 1, 0x02 + (select?8:0));
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + ioff + 2, 0x11 + (select?16:0) + icon);
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + ioff + 3, 0x13 + (select?16:0) + icon);
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + ioff + 4, 0x03 + (select?8:0));
		set_vram_byte(TILEMAP0 + 17*BUFFER_WIDTH + ioff + 5, 0x00);
		
	}

	decompress_sprite(TILEBLOCK1 + 0x0000, font_data);
	decompress_sprite(TILEBLOCK2 + 0x0000, encounter_ui_data);
	decompress_sprite(TILEBLOCK2 + 0x0400, enemy_sprite);


	// There's an interrupt here so that the bottom 2 tiles are offset by 4 pixels.
	// The reason for this is that this lets me save a couple tiles on the icons.
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


	while (true) {
		vsync();
	}
}
