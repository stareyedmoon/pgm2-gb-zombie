#include <stdint.h>

#include <game/encounter.h>

#include <defines.h>

static void draw_initial_background(void) {
    // Enemy area
    for (uint16_t y = 0; y < 9; y += 1) {
        for (uint16_t x = 0; x < BUFFER_WIDTH; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x00);
        }
    }
    // Text area
    for (uint16_t y = 10; y < 28; y += 1) {
        for (uint16_t x = 0; x < BUFFER_WIDTH; x += 1) {
            //set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x80);
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x80 + y - 9);
        }
    }
    // Player area
    for (uint16_t y = 29; y < BUFFER_HEIGHT; y += 1) {
        for (uint16_t x = 0; x < BUFFER_HEIGHT; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x, 0x08);
        }
    }
}
static void draw_initial_edges(void) {
    for (uint16_t x = 0; x < 32; x += 1) {
        set_vram_byte(TILEMAP0 + 9*BUFFER_WIDTH + x, 0x02);
        set_vram_byte(TILEMAP0 + 28*BUFFER_WIDTH + x, 0x0A);
    }

    set_vram_byte(TILEMAP0 + 9*BUFFER_WIDTH + 0, 0x01);
    set_vram_byte(TILEMAP0 + 9*BUFFER_WIDTH + 19, 0x03);
    set_vram_byte(TILEMAP0 + 28*BUFFER_WIDTH + 0, 0x09);
    set_vram_byte(TILEMAP0 + 28*BUFFER_WIDTH + 19, 0x0B);
}
static void draw_initial_bars(void) {
    for (uint16_t x = 0; x < 8; x += 1) {
        set_vram_byte(TILEMAP0 + 8*BUFFER_WIDTH + x + 6, 0x30 + x);
        set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + x + 6, 0x38 + x);
    }

    set_vram_byte(TILEMAP0 + 8*BUFFER_WIDTH + 5, 0x1E);
    set_vram_byte(TILEMAP0 + 8*BUFFER_WIDTH + 14, 0x1F);
    set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 5, 0x16);
    set_vram_byte(TILEMAP0 + 31*BUFFER_WIDTH + 14, 0x17);
}
static void draw_initial_enemy(void) {
    for (uint16_t y = 0; y < 8; y += 1) {
        for (uint16_t x = 0; x < 6; x += 1) {
            set_vram_byte(TILEMAP0 + y*BUFFER_WIDTH + x + 7, 0x50 + y*6 + x);
        }
    }
}
static void draw_initial_buttons(void) {
    static const uint8_t button_tiles_left[] = {
        0x10, 0x11, 0x13, 0x11, 0x12,
        0x20, 0x21, 0x14, 0x24, 0x25,
        0x22, 0x23, 0x15, 0x26, 0x27,
    };
    static const uint8_t button_tiles_right[] = {
        0x10, 0x11, 0x13, 0x11, 0x12,
        0x28, 0x29, 0x14, 0x2C, 0x2D,
        0x2A, 0x2B, 0x15, 0x2E, 0x2F,
    };

    for (uint16_t y = 0; y < 3; y += 1) {
        for (uint16_t x = 0; x < 5; x += 1) {
            set_vram_byte(TILEMAP0 + (y+29)*BUFFER_WIDTH + x + 0, button_tiles_left[y*5 + x]);
            set_vram_byte(TILEMAP0 + (y+29)*BUFFER_WIDTH + x + 15, button_tiles_right[y*5 + x]);
        }
    }
}


void encounter_draw_initial_ui(void) {
    draw_initial_background();
    draw_initial_bars();
    draw_initial_edges();
    draw_initial_enemy();
    draw_initial_buttons();
}
