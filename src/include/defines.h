#include <gb/gb.h>

#ifndef _defines_h_INCLUDE
#define _defines_h_INCLUDE

#ifndef NULL
#define NULL ((void*)0)
#endif

#define BUFFER_WIDTH DEVICE_SCREEN_BUFFER_WIDTH
#define BUFFER_WIDTH_2 (DEVICE_SCREEN_BUFFER_WIDTH/2)

#define BUFFER_HEIGHT DEVICE_SCREEN_BUFFER_HEIGHT
#define BUFFER_HEIGHT_2 (DEVICE_SCREEN_BUFFER_HEIGHT/2)

#define SCREEN_WIDTH DEVICE_SCREEN_WIDTH
#define SCREEN_WIDTH_2 (DEVICE_SCREEN_WIDTH/2)

#define SCREEN_HEIGHT DEVICE_SCREEN_HEIGHT
#define SCREEN_HEIGHT_2 (DEVICE_SCREEN_HEIGHT/2)

// At indexes 0 to 127 in low addressing mode
#define TILEBLOCK0 ((uint8_t*)0x8000)
// Always at indexes 128 to 255
#define TILEBLOCK1 ((uint8_t*)0x8800)
// At indexes 0 to 127 in high addressing mode
#define TILEBLOCK2 ((uint8_t*)0x9000)

#define TILEMAP0 ((uint8_t*)0x9800)
#define TILEMAP1 ((uint8_t*)0x9C00)

// Base index for Player tiles
// There are the ones used for drawing the player.
// Should be maybe 32 of them, I guess? Might change that later.
#define TILE_BASE_PLAYER 0

// 0x80 : LCD and PPU enable (ONLY MODIFY DURING VBLANK)
// 0x40 : Window tilemap area
// 0x20 : Window enable
// 0x10 : BG and Window tile data area
// 0x08 : BG Tilemap area
// 0x04 : Sprite size
// 0x02 : Sprite enable
// 0x01 : BG and Window enable
#define LCD_CTRL *((uint8_t*)0xFF40)
// 0x80 : Unused
// 0x40 : LYC Interrupt Enable
// 0x20 : Mode 2 Interrupt Enable
// 0x10 : Mode 1 Interrupt Enable
// 0x08 : Mode 0 Interrupt Enable
// 0x04 : (RO) LYC==LY
// 0x03 : (RO) PPU Mode
#define LCD_STAT *((uint8_t*)0xFF41)
// Background viewport position Y
#define LCD_SCY *((uint8_t*)0xFF42)
// Background viewport position X
#define LCD_SCX *((uint8_t*)0xFF43)
// Current scanline. Ranges from 0 to 153, where 144 to 153 are VBlank.
#define LCD_LY *((uint8_t*)0xFF44)
// If LYC interrupt is enabled, a STAT interrupt will be requested when this is equal to LY.
#define LCD_LYC *((uint8_t*)0xFF45)
// Background and Window tile palette
#define LCD_BGP *((uint8_t*)0xFF47)
// Sprite palette 0
#define LCD_OBP0 *((uint8_t*)0xFF48)
// Sprite palette 1
#define LCD_OBP1 *((uint8_t*)0xFF49)
// Window Y postition
#define LCD_WY *((uint8_t*)0xFF4A)
// Window X position +7
#define LCD_WX *((uint8_t*)0xFF4B)

// 0xC0 : Unused
// 0x20 : Select buttons
// 0x10 : Select D-pad
// 0x08 : (RO)  Start / Down
// 0x04 : (RO) Select / Up
// 0x02 : (RO)      B / Left
// 0x01 : (RO)      A / Right
#define JOYPAD *((uint8_t*)0xFF00)

#define INTERRUPT_FLAG *((uint8_t*)0xFF0F)
#define INTERRUPT_ENABLE *((uint8_t*)0xFFFF)

#endif // _defines_h_INCLUDE
