#include <gb/gb.h>

#ifndef _defines_h_INCLUDE
#define _defines_h_INCLUDE


#define BUFFER_WIDTH DEVICE_SCREEN_BUFFER_WIDTH
#define BUFFER_WIDTH_2 (DEVICE_SCREEN_BUFFER_WIDTH/2)

#define BUFFER_HEIGHT DEVICE_SCREEN_BUFFER_HEIGHT
#define BUFFER_HEIGHT_2 (DEVICE_SCREEN_BUFFER_HEIGHT/2)

#define SCREEN_WIDTH DEVICE_SCREEN_WIDTH
#define SCREEN_WIDTH_2 (DEVICE_SCREEN_WIDTH/2)

#define SCREEN_HEIGHT DEVICE_SCREEN_HEIGHT
#define SCREEN_HEIGHT_2 (DEVICE_SCREEN_HEIGHT/2)


#define TILEBLOCK0 ((uint8_t*)0x8000)
#define TILEBLOCK1 ((uint8_t*)0x8800)
#define TILEBLOCK2 ((uint8_t*)0x9000)

#define TILEMAP0 ((uint8_t*)0x9800)
#define TILEMAP1 ((uint8_t*)0x9C00)

// Base index for Player tiles
// There are the ones used for drawing the player.
// Should be maybe 32 of them, I guess? Might change that later.
#define TILE_BASE_PLAYER 0

#endif // _defines_h_INCLUDE
