#include <gb/gb.h>

#include <stdint.h>

#include <engine.h>
#include <player.h>

#include "tile.h"
#include "testmap.h"
#include "testtiles.h"
#include <font.h>

#include "defines.h"
#include "debug.h"

void main(void) {
    waitpadup();
    waitpad(0xff);

    set_sprite_data(TILE_BASE_PLAYER, 6, test);
    
    engine_init();

    engine_set_map(testmap);
    
    Player player = player_new("PLAYER", PLAYER_ARCHETYPE_BASIC);
    player.sprite_index[0] = engine_alloc_sprite();
    player.sprite_index[1] = engine_alloc_sprite();
    player.x = 10;
    player.y = 8;

    set_tile_data(0, 32, testtiles, 0x90);

    int8_t dx = 0;
    int8_t dy = 0;
    while (1) {
        uint8_t inp = joypad();
            
        // Get movement direction
        dx = ((inp&J_RIGHT)!=0) - ((inp&J_LEFT)!=0);
        dy = ((inp&J_DOWN)!=0) - ((inp&J_UP)!=0);
        
        player_update_sprite(&player, dx, dy);
        
        switch (sys_time & 7) {
        case 0:
            // Check X collision
            if (engine_get_collision(player.x + dx, player.y)) dx = 0;

            // Check Y collision
            if (engine_get_collision(player.x, player.y + dy)) dy = 0;            

            // If moving both horizontally and vertically, alternate between them every movement step
            if (dx != 0 && dy != 0) {
                if (sys_time & 8) dx = 0;
                else dy = 0;
            }

            player.x += dx;
            player.y += dy;

            engine_move_camera(player.x, player.y);
            engine_move_sprite(player.sprite_index, player.x, player.y);

            break;
        }

        vsync();
    }
}
