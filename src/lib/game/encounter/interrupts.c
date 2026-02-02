#include <gb/gb.h>

#include <game/encounter.h>
#include <engine.h>

#include <defines.h>
#include <general.h>

int8_t encounter_enemy_animation[16] = {0};
uint8_t encounter_enemy_animation_index = 16;

uint8_t encounter_text_scroll = 0;


uint8_t encounter_animation_damage_animation_index = 120;

uint8_t encounter_animation_damage_numbers[5] = {0,0,0,0,0};
uint8_t encounter_animation_damage_crit = 0;

static uint8_t encounter_animation_damage_sprites[5] = {255,255,255,255,255};

static uint8_t encounter_animation_damage_x = 0;
static uint8_t encounter_animation_damage_y = 0;

static int8_t encounter_animation_velocity_x = 0;
static int8_t encounter_animation_velocity_y = 0;

static const uint8_t encounter_animation_damage_floor = 240;


uint8_t encounter_held = 0;
uint8_t encounter_just_pressed = 0;
uint8_t encounter_just_released = 0;


static void lcd_int_handler(void) {
    disable_interrupts();
	if (LCD_LYC == 0) { // Enemy
        LCD_SCX = encounter_enemy_animation_index < 16 ? encounter_enemy_animation[encounter_enemy_animation_index] : 0;
        LCD_SCY = 0;
        
        LCD_LYC = 60;

		uint8_t joy = joypad();
		encounter_just_pressed = joy & ~encounter_held;
		encounter_just_released = encounter_held & ~joy;
		encounter_held = joy;
	}
	else if (LCD_LYC == 60) { // Enemy health and turn bar
        LCD_SCX = 0;
        LCD_SCY = 3;
        
        LCD_LYC = 68;
	}
    else if (LCD_LYC == 68) { // Text top border
        LCD_SCX = 0;
        LCD_SCY = 8;

        LCD_LYC = 71;
    }
    else if (LCD_LYC == 71) { // Text
        LCD_SCX = 0;
        LCD_SCY = 8 + encounter_text_scroll * 8;

        LCD_LYC = 119;
    }
    else if (LCD_LYC == 119) { // Text bottom border
        LCD_SCX = 0;
        LCD_SCY = 104;

        LCD_LYC = 124;
    }
    else if (LCD_LYC == 124) {
        LCD_SCX = 0;
        LCD_SCY = 112;

        LCD_LYC = 0;

		if (encounter_animation_damage_animation_index == 0) {
			encounter_animation_damage_x = 0;
			encounter_animation_damage_y = 144;
			encounter_animation_velocity_x = 6;
			encounter_animation_velocity_y = -9;
			
			set_vram_byte(&LCD_OBP1, 0xE4);
		
			for (uint8_t i = 0; i < 5; i++) {
				if (encounter_animation_damage_sprites[i] == 255) {
					encounter_animation_damage_sprites[i] = engine_alloc_sprite();
				}
			
				set_vram_byte(OAM + encounter_animation_damage_sprites[i]*4 + 2, encounter_animation_damage_numbers[i]);
				set_vram_byte(OAM + encounter_animation_damage_sprites[i]*4 + 3, 0x10);
			
			}
		}

		if (encounter_enemy_animation_index < 16) encounter_enemy_animation_index += 1;

		if (encounter_animation_damage_animation_index < 120) {
			encounter_animation_velocity_y += 1;

			encounter_animation_damage_x += encounter_animation_velocity_x;
			encounter_animation_damage_y += encounter_animation_velocity_y;

			if (encounter_animation_damage_y > encounter_animation_damage_floor) {
				if (encounter_animation_velocity_y > 3) {
					encounter_animation_damage_y = 2*encounter_animation_damage_floor - encounter_animation_damage_y;
					encounter_animation_velocity_y = -(encounter_animation_velocity_y / 2);
				}
				else {
					encounter_animation_velocity_y = 0;
					encounter_animation_damage_y = encounter_animation_damage_floor;
				}
				encounter_animation_velocity_x = (encounter_animation_velocity_x * 5) / 7;
			}
		
			for (uint8_t i = 0; i < 5; i += 1) {
				set_vram_byte(OAM + encounter_animation_damage_sprites[i]*4 + 0, encounter_animation_damage_y / 4);
				set_vram_byte(OAM + encounter_animation_damage_sprites[i]*4 + 1, encounter_animation_damage_x / 4 + 60 + 8*i);
		
				//set_vram_byte(OAM + encounter_animation_damage_sprites[i]*4 + 0, 32);
				//set_vram_byte(OAM + encounter_animation_damage_sprites[i]*4 + 1, 32 + i*8);
			}

			encounter_animation_damage_animation_index += 1;

			if (encounter_animation_damage_animation_index == 90) {
				set_vram_byte(&LCD_OBP1, 0x92);
			}
			if (encounter_animation_damage_animation_index == 105) {
				set_vram_byte(&LCD_OBP1, 0x40);

			}

			if (encounter_animation_damage_animation_index == 120) {
				for (uint8_t i = 0; i < 5; i++) {
					engine_free_sprite(encounter_animation_damage_sprites[i]);
					encounter_animation_damage_sprites[i] = 255;
				}
			}
		}

		rand8();
    }
	enable_interrupts();
}
static void vblank_int_handler(void) {
    rand8();
}

void encounter_enable_interrupts(void) {
    disable_interrupts();
	
    LCD_SCX = 0;
	LCD_SCY = 0;

	LCD_LYC = 0;
	
	CRITICAL {
		LCD_STAT &= 0x83;
		LCD_STAT |= 0x40;
		add_LCD(lcd_int_handler);
		add_LCD(nowait_int_handler);

        //add_VBL(vblank_int_handler);
	}
	
	enable_interrupts();
    set_interrupts(IE_REG | LCD_IFLAG);
}
void encounter_disable_interrupts(void) {
    disable_interrupts();

    LCD_SCX = 0;
    LCD_SCY = 0;

    LCD_LYC = 0;

    CRITICAL {
        LCD_STAT &= ~0x40;
        remove_LCD(nowait_int_handler);
        remove_LCD(lcd_int_handler);

        //remove_VBL(vblank_int_handler);
    }

    enable_interrupts();

    set_interrupts(IE_REG & ~LCD_IFLAG);
}
