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

void main(void) {
    SPRITES_8x16;

    set_sprite_data(0, 8, test);
    
    uint8_t x = 80;
    uint8_t y = 100;

    set_tile_data(0, 64, testtiles, 0x90);
    for (uint16_t i = 0; i < 32*32; i += 1) {
        //tilemap[i] = testmap[i] & 0x1f;
        set_vram_byte(tilemap+i, (testmap[i] & 0x1f));
    }
    for (uint16_t i = 32*24; i < 32*32; i += 1) {
        set_vram_byte(tilemap+i, random() & 0x3f);
    }
    
    /*for (uint16_t i = 16*288; i < 16*320; i += 1) {
        set_vram_byte(tiles+i, random());
    }*/

    SHOW_SPRITES;
    SHOW_BKG;

    //set_bkg_tiles(0, 0, 32, 24, )

    while (1) {
        uint8_t inp = joypad();
        
        int8_t dx = 0;
        int8_t dy = 0;
        if (inp & J_UP) {
            dy--;
            set_sprite_tile(0, 4);
            set_sprite_prop(0, 0);
        }
        if (inp & J_DOWN) {
            dy++;
            set_sprite_tile(0, 0);
            set_sprite_prop(0, 0);
        }
        if (inp & J_RIGHT) {
            dx++;
            set_sprite_tile(0, 2);
            set_sprite_prop(0, 0);
        }
        if (inp & J_LEFT) {
            dx--;
            set_sprite_tile(0, 2);
            set_sprite_prop(0, S_FLIPX);
        }

        if (testmap[((y/8)*32 + (x+dx)/8) & 0x3FF] & 0x20) dx = 0;
        x += dx;
        if (testmap[(((y+dy)/8)*32 + x/8) & 0x3FF] & 0x20) dy = 0;
        y += dy;

        move_sprite(0, 84, 76);
        move_bkg(x - 80, y - 75);

        vsync();
    }
}
