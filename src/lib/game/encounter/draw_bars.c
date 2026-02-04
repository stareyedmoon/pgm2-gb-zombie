#include <stdint.h>

#include <game/encounter.h>

#include <defines.h>
#include <general.h>

void encounter_draw_initial_bar_tiles(void) {
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
}

void encounter_draw_player_health_bar(uint16_t health, uint16_t max_health) {
    static uint8_t current_fill = 0;

    uint8_t fill = (health * 64 + max_health - 1) / max_health;
    
    if (fill == current_fill) return;
    else if (fill < current_fill) {
        for (uint8_t i = current_fill - 1; i >= fill && i != 255; i -= 1) {
            uint8_t v = 0xFF >> (i & 7);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (i & 0xF8)*2 + 2, v);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (i & 0xF8)*2 + 3, v);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (i & 0xF8)*2 + 4, v);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (i & 0xF8)*2 + 5, v);

            vsync();
        }
    }
    else { // fill > current_fill
        for (uint8_t i = current_fill & 0xF8; i < (fill & 0xF8); i += 8) {
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*2 + 2, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*2 + 3, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*2 + 4, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*2 + 5, 0x00);
        }
        if (fill & 7) {
            uint8_t part = 0xFF >> (fill & 7);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (fill & 0xF8)*2 + 2, part);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (fill & 0xF8)*2 + 3, part);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (fill & 0xF8)*2 + 4, part);
            set_vram_byte(TILEBLOCK2 + 0x0380 + (fill & 0xF8)*2 + 5, part);
        }
    }

    current_fill = fill;
}
void encounter_draw_enemy_health_bar(uint16_t health, uint16_t max_health) {
    static uint8_t current_fill = 0;

    uint8_t fill = (health * 64 + max_health - 1) / max_health;
    
    if (fill == current_fill) return;
    else if (fill < current_fill) {
        for (uint8_t i = current_fill - 1; i >= fill && i != 255; i -= 1) {
            uint8_t v = 0xFF >> (i & 7);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (i & 0xF8)*2 + 2, v);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (i & 0xF8)*2 + 3, v);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (i & 0xF8)*2 + 4, v);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (i & 0xF8)*2 + 5, v);

            vsync();
        }
    }
    else { // fill > current_fill
        for (uint8_t i = current_fill & 0xF8; i < (fill & 0xF8); i += 8) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*2 + 2, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*2 + 3, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*2 + 4, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*2 + 5, 0x00);
        }
        if (fill & 7) {
            uint8_t part = 0xFF >> (fill & 7);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (fill & 0xF8)*2 + 2, part);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (fill & 0xF8)*2 + 3, part);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (fill & 0xF8)*2 + 4, part);
            set_vram_byte(TILEBLOCK2 + 0x0300 + (fill & 0xF8)*2 + 5, part);
        }
    }

    current_fill = fill;
}

void encounter_draw_player_turn_bar(uint8_t turn_counter, bool full) {
    if (full) {
        for (uint8_t i = 0; i < 8; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 9, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 11, 0x00);
        }
    }
    else {
        uint8_t full_bars = turn_counter / 32;
        uint8_t partial = (0xFF) >> ((turn_counter / 4) % 8);

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
void encounter_draw_enemy_turn_bar(uint8_t turn_counter, bool full) {
    if (full) {
        for (uint8_t i = 0; i < 8; i += 1) {
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 9, 0x00);
            set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 11, 0x00);
        }
    }
    else {
        uint8_t full_bars = turn_counter / 32;
        uint8_t partial = (0xFF) >> ((turn_counter / 4) % 8);

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
