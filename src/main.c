#include <gb/gb.h>

#include <stdint.h>

#include "tile.h"
#include "testmap.h"
#include "testtiles.h"



#define BUFFER_WIDTH DEVICE_SCREEN_BUFFER_WIDTH
#define BUFFER_WIDTH_2 (DEVICE_SCREEN_BUFFER_WIDTH/2)

#define BUFFER_HEIGHT DEVICE_SCREEN_BUFFER_HEIGHT
#define BUFFER_HEIGHT_2 (DEVICE_SCREEN_BUFFER_HEIGHT/2)

#define SCREEN_WIDTH DEVICE_SCREEN_WIDTH
#define SCREEN_WIDTH_2 (DEVICE_SCREEN_WIDTH/2)

#define SCREEN_HEIGHT DEVICE_SCREEN_HEIGHT
#define SCREEN_HEIGHT_2 (DEVICE_SCREEN_HEIGHT/2)



#define PLAYER_SPRITE 0

#define TILE_PLAYER_DOWN 0
#define TILE_PLAYER_SIDE 2
#define TILE_PLAYER_UP 4

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
    SHOW_SPRITES;
    SHOW_BKG;


    const unsigned char* map = testmap;
    uint16_t map_width = testmapWidth;
    uint16_t map_height = testmapHeight;


    set_sprite_data(TILE_PLAYER_DOWN, 6, test);
    
    uint8_t x = 10;
    uint8_t y = 8;

    const uint8_t buffer_max_x = map_width - BUFFER_WIDTH;
    const uint8_t buffer_max_y = map_height - BUFFER_HEIGHT;

    uint8_t buffer_x = 0;
    uint8_t buffer_y = 0;

    set_tile_data(0, 32, testtiles, 0x90);
    for (uint16_t y = 0; y < min(BUFFER_HEIGHT, map_height); y += 1) {
        for (uint16_t x = 0; x < min(BUFFER_WIDTH, map_width); x += 1) {
            set_vram_byte(tilemap+(y*BUFFER_WIDTH)+x, map[(y*map_width)+x] & 0x1f);
        }
    }

    int8_t dx = 0;
    int8_t dy = 0;
    while (1) {
        switch (sys_time & 7) {
        case 0:
            uint8_t inp = joypad();
            
            // Get movement direction
            dx = ((inp&J_RIGHT)!=0) - ((inp&J_LEFT)!=0);
            dy = ((inp&J_DOWN)!=0) - ((inp&J_UP)!=0);

            // Check X collision
            if (map[(y*map_width + (x+dx))] & 0x20) dx = 0;
            else if (x == 0 && dx == -1) dx = 0;
            else if (x == map_width-1 && dx == 1) dx = 0;
            
            // Check Y collision
            if (map[((y+dy)*map_width + x)] & 0x20) dy = 0;
            else if (y == 1 && dy == -1) dy = 0; // Minimum Y is 1 because at Y=0 player sprite is completely off-screen.
            else if (y == map_height-1 && dy == 1) dy = 0; 


            // If moving both horizontally and vertically, alternate between them every movement step
            if (dx != 0 && dy != 0) {
                if (sys_time & 8) dx = 0;
                else dy = 0;
            }
            
            // Set player sprite to point in direction of movement
            if (dx == 1) {
                set_sprite_tile(PLAYER_SPRITE, TILE_PLAYER_SIDE);
                set_sprite_prop(PLAYER_SPRITE, 0);
            }
            else if (dx == -1) {
                set_sprite_tile(PLAYER_SPRITE, TILE_PLAYER_SIDE);
                set_sprite_prop(PLAYER_SPRITE, S_FLIPX);
            }
            else if (dy == 1) {
                set_sprite_tile(PLAYER_SPRITE, TILE_PLAYER_DOWN);
                set_sprite_prop(PLAYER_SPRITE, 0);
            }
            else if (dy == -1) {
                set_sprite_tile(PLAYER_SPRITE, TILE_PLAYER_UP);
                set_sprite_prop(PLAYER_SPRITE, 0);
            }
            break;
            
        case 1:
            // Load map into tilemap if necessary
            // TODO - Make sure there aren't any off-by-one errors
            if (dx != 0 || dy != 0) {
                if (map_width > BUFFER_WIDTH && dx != 0) {
                    // Moving left
                    if (buffer_x != 0 && dx == -1 && x == (buffer_x + SCREEN_WIDTH_2 + 1)) {
                        buffer_x -= 1;

                        uint8_t src_x = buffer_x;
                        uint8_t dest_x = src_x % BUFFER_WIDTH;

                        for (uint16_t cy = 0; cy < min(BUFFER_HEIGHT, map_height); cy += 1) {
                            set_vram_byte(tilemap+(cy*BUFFER_WIDTH)+dest_x, map[((cy+buffer_y)*map_width)+src_x] & 0x1f);
                        }
                    }
                    // Moving right
                    else if (buffer_x < buffer_max_x && dx == 1 && x == (buffer_x + BUFFER_WIDTH - SCREEN_WIDTH_2 - 2)) {
                        buffer_x += 1;

                        uint8_t src_x = buffer_x + BUFFER_WIDTH - 1;
                        uint8_t dest_x = src_x % BUFFER_WIDTH;

                        for (uint16_t cy = 0; cy < min(BUFFER_HEIGHT, map_height); cy += 1) {
                            set_vram_byte(tilemap+(cy*BUFFER_WIDTH)+dest_x, map[((cy+buffer_y)*map_width)+src_x] & 0x1f);
                        }
                    }
                }
                if (map_height > BUFFER_HEIGHT && dy != 0) {
                    // Moving down
                    if (buffer_y != 0 && dy == -1 && y == (buffer_y + SCREEN_HEIGHT_2 + 1)) {
                        buffer_y -= 1;

                        uint8_t src_y = buffer_y;
                        uint8_t dest_y = src_y % BUFFER_HEIGHT;

                        for (uint16_t cx = 0; cx < min(BUFFER_WIDTH, map_width); cx += 1) {
                            set_vram_byte(tilemap+(dest_y*BUFFER_WIDTH)+cx, map[(src_y*map_width)+(cx+buffer_x)] & 0x1f);
                        }
                    }
                    // Moving up
                    else if (buffer_y < buffer_max_y && dy == 1 && y == (buffer_y + BUFFER_HEIGHT - SCREEN_HEIGHT_2 - 2)) {
                        buffer_y += 1;

                        uint8_t src_y = buffer_y + BUFFER_HEIGHT - 1;
                        uint8_t dest_y = src_y % BUFFER_HEIGHT;

                        for (uint16_t cx = 0; cx < min(BUFFER_WIDTH, map_width); cx += 1) {
                            set_vram_byte(tilemap+(dest_y*BUFFER_WIDTH)+cx, map[(src_y*map_width)+(cx+buffer_x)] & 0x1f);
                        }
                    }
                }
            }
            break;

        case 2:
            x = x + dx;
            y = y + dy;

            // Calculate offset of camera (bkg scroll in tiles)
            uint8_t camera_tile_offset_x = max(min(x, map_width - SCREEN_WIDTH_2 - 1), SCREEN_WIDTH_2) - SCREEN_WIDTH_2;
            uint8_t camera_tile_offset_y = max(min(y, map_height - SCREEN_HEIGHT_2), SCREEN_HEIGHT_2 + 1) - SCREEN_HEIGHT_2;
            
            // Calculate offset of player sprite
            uint8_t player_tile_offset_x = x - camera_tile_offset_x;
            uint8_t player_tile_offset_y = y - camera_tile_offset_y;
            
            move_sprite(PLAYER_SPRITE, player_tile_offset_x*8 + DEVICE_SPRITE_PX_OFFSET_X/2, player_tile_offset_y*8 + DEVICE_SPRITE_PX_OFFSET_Y/2);
            move_bkg(camera_tile_offset_x*8 + 4, camera_tile_offset_y*8 - 3);
            break;
        }

        vsync();
    }
}
