#include <gb/gb.h>

#include <stdint.h>

#include "tile.h"
#include "testmap.h"
#include "testtiles.h"

static uint8_t* const tilemap = (uint8_t*)0x9800;
static uint8_t* const tiles = (uint8_t*)0x8000;

uint8_t random(void) {
    static uint32_t state = 0x5944259E;

    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;

    return state & 0xff;
}

uint8_t min(uint8_t a, uint8_t b) {
    return a < b ? a : b;
}
uint8_t max(uint8_t a, uint8_t b) {
    return a < b ? b : a;
}

void main(void) {
    SPRITES_8x16;

    set_sprite_data(0, 8, test);
    
    uint8_t x = 10;
    uint8_t y = 13;

    set_tile_data(0, 64, testtiles, 0x90);
    for (uint16_t i = 0; i < 32*32; i += 1) {
        set_vram_byte(tilemap+i, (testmap[i] & 0x1f));
    }

    SHOW_SPRITES;
    SHOW_BKG;

    while (1) {
        uint8_t inp = joypad();
        
        int8_t dx = 0;
        int8_t dy = 0;
        if (inp & J_UP) {
            dy -= 1;
            set_sprite_tile(0, 4);
            set_sprite_prop(0, 0);
        }
        if (inp & J_DOWN) {
            dy += 1;
            set_sprite_tile(0, 0);
            set_sprite_prop(0, 0);
        }
        if (inp & J_RIGHT) {
            dx += 1;
            set_sprite_tile(0, 2);
            set_sprite_prop(0, 0);
        }
        if (inp & J_LEFT) {
            dx -= 1;
            set_sprite_tile(0, 2);
            set_sprite_prop(0, S_FLIPX);
        }

        if ((sys_time & 7) == 0) {
            if (testmap[(y*32 + ((x+dx)&31))] & 0x20) dx = 0;
            x = (x + dx) & 31;
            if (testmap[((y+dy)*32 + x) & 0x3FF] & 0x20) dy = 0;
            y = (y + dy) & 31;
        }

        uint8_t camera_tile_offset_x = max(min(x, testmapWidth - DEVICE_SCREEN_WIDTH/2 - 1), DEVICE_SCREEN_WIDTH/2) - DEVICE_SCREEN_WIDTH / 2;
        uint8_t camera_tile_offset_y = max(min(y, testmapHeight - DEVICE_SCREEN_HEIGHT/2 - 1), DEVICE_SCREEN_HEIGHT/2) - DEVICE_SCREEN_HEIGHT / 2;

        uint8_t player_tile_offset_x = x - camera_tile_offset_x;
        uint8_t player_tile_offset_y = y - camera_tile_offset_y;

        move_sprite(0, player_tile_offset_x*8 + DEVICE_SPRITE_PX_OFFSET_X/2, player_tile_offset_y*8 - 4 + DEVICE_SPRITE_PX_OFFSET_Y/2);
        move_bkg(camera_tile_offset_x*8 + 4, camera_tile_offset_y*8 + 1);

        vsync();
    }
}
