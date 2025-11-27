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
    uint8_t fill = (health * 64) / max_health;
    
    uint8_t full_bars = MIN(fill / 8, 7);
    uint8_t partial = (0xFF) >> (fill % 8);
    if (health == max_health) partial = 0x00;

    for (uint8_t i = 0; i < full_bars; i += 1) {
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 2, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 3, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 4, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 5, 0x00);
    }
    set_vram_byte(TILEBLOCK2 + 0x0380 + full_bars*16 + 2, partial);
    set_vram_byte(TILEBLOCK2 + 0x0380 + full_bars*16 + 3, partial);
    set_vram_byte(TILEBLOCK2 + 0x0380 + full_bars*16 + 4, partial);
    set_vram_byte(TILEBLOCK2 + 0x0380 + full_bars*16 + 5, partial);
    for (uint8_t i = full_bars + 1; i < 8; i += 1) {
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 2, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 3, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 4, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0380 + i*16 + 5, 0xFF);
    }
}
void encounter_draw_enemy_health_bar(uint16_t health, uint16_t max_health) {
    uint8_t fill = (health * 64) / max_health;
    
    uint8_t full_bars = MIN(fill / 8, 7);
    uint8_t partial = (0xFF) >> (fill % 8);
    if (health == max_health) partial = 0x00;

    for (uint8_t i = 0; i < full_bars; i += 1) {
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 2, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 3, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 4, 0x00);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 5, 0x00);
    }
    set_vram_byte(TILEBLOCK2 + 0x0300 + full_bars*16 + 2, partial);
    set_vram_byte(TILEBLOCK2 + 0x0300 + full_bars*16 + 3, partial);
    set_vram_byte(TILEBLOCK2 + 0x0300 + full_bars*16 + 4, partial);
    set_vram_byte(TILEBLOCK2 + 0x0300 + full_bars*16 + 5, partial);
    for (uint8_t i = full_bars + 1; i < 8; i += 1) {
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 2, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 3, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 4, 0xFF);
        set_vram_byte(TILEBLOCK2 + 0x0300 + i*16 + 5, 0xFF);
    }
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
