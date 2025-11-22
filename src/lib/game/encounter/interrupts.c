#include <gb/gb.h>

#include <game/encounter.h>

#include <defines.h>
#include <general.h>

int8_t encounter_enemy_animation = 0;
uint8_t encounter_text_scroll = 0;

static void lcd_int_handler(void) {
    disable_interrupts();
	if (LCD_LYC == 0) { // Enemy
        LCD_SCX = encounter_enemy_animation;
        LCD_SCY = 0;
        
        LCD_LYC = 60;
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
}
