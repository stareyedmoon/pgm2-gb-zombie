#include <gb/gb.h>

#include <stdint.h>

#include "tile.h"

void main(void) {
    SPRITES_8x16;
    set_sprite_data(0, 8, test);
    SHOW_SPRITES;

    uint8_t x = 84;
    uint8_t y = 80;

    while (1) {

        uint8_t inp = joypad();
        if (inp & J_RIGHT) {
            x++;
            set_sprite_tile(0, 2);
        }
        if (inp & J_LEFT) {
            x--;
            set_sprite_tile(0, 4);
        }
        if (inp & J_UP) {
            y--;
            set_sprite_tile(0, 6);
        }
        if (inp & J_DOWN) {
            y++;
            set_sprite_tile(0, 0);
        } 

        move_sprite(0, x, y);

        wait_vbl_done();
    }
}
