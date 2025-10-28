#include <gb/gb.h>

#include <engine.h>
#include <defines.h>
#include <font.h>

#include <debug.h>

/******************************************************************************\
****  Internal Variables  ******************************************************
\******************************************************************************/

/// @brief Frames between each character in text rendering
uint8_t e_text_speed = 0;

/// @brief Pointer to 
static const uint8_t* e_map = NULL;
static const uint16_t e_map_width = 0;
static const uint16_t e_map_height = 0;

static uint16_t e_map_loaded_left = 0;
static uint16_t e_map_loaded_top = 0;

static uint16_t e_map_loaded_right = 0;
static uint16_t e_map_loaded_down = 0;


static uint8_t e_random_x = 0;
static uint8_t e_random_y = 0;
static uint8_t e_random_z = 0;
static uint8_t e_random_a = 1;

/******************************************************************************\
****  Internal Functions  ******************************************************
\******************************************************************************/

/// @brief Load mapdata into tilemap
/// @param dest Pointer to tilemap to render into
/// @param src Pointer to map data to load from
/// @param map_width Width of map data
/// @param map_height Height of map data
/// @param x Left edge of map data to load
/// @param y Top edge of map data to load
/// @param w Width of map data to load
/// @param h Height of map data to load
static void load_map(uint8_t* const restrict dest, const uint8_t* const restrict src,
	          uint16_t map_width, uint16_t map_height, uint16_t x, uint16_t y,
			  uint16_t w, uint16_t h) {
    ASSERT(w <= BUFFER_WIDTH, "Map load width should not exceed buffer width.");
    ASSERT(h <= BUFFER_HEIGHT, "Map load height should not exceed buffer height.");    
    ASSERT((x+w) <= map_width, "Width overrun during attempt to load map.");
    ASSERT((y+h) <= map_height, "Height overrrun during attempt to load map.");

    for (uint16_t src_y = y; src_y < (y+h); src_y += 1) {
        uint16_t dest_y = src_y % BUFFER_HEIGHT;
        for (uint16_t src_x = x; src_x < (x+w); src_x += 1) {
            uint16_t dest_x = src_x % BUFFER_WIDTH;

            set_vram_byte(dest + (dest_y*BUFFER_WIDTH) + dest_x, src[(src_y*map_width) + src_x] & 0x1F);
        }
    }
}

/// @brief Convert an ASCII character to internal representation
/// @param base_tile Index of first tile of font
/// @param c ASCII character
/// @return Index of character tile
/// @note Returns a question mark if character doesn't exist in character set
static int8_t char_to_tile(uint8_t base_tile, char c) {
	if (c == ' ') return base_tile + 0x00;
	if (c >= 'a' && c <= 'z') return base_tile + 0x01 + c - 'a'; // Lowercase gets converted to uppercase
	if (c >= 'A' && c <= 'Z') return base_tile + 0x01 + c - 'A';
	if (c == '-') return base_tile + 0x1B;
	if (c == '.') return base_tile + 0x1C;
	if (c == ',') return base_tile + 0x1D;
	if (c == '!') return base_tile + 0x1E;
	if (c == '?') return base_tile + 0x1F;
	if (c >= '0' && c <= '9') return base_tile + 0x20 + c - '0';
	if (c == '\'') return base_tile + 0x2A;
	if (c == '"') return base_tile + 0x2B;
	if (c == '(') return base_tile + 0x2C;
	if (c == ')') return base_tile + 0x2D;

	ASSERT(1, "Invalid character conversion.");
	return base_tile + 0x1F;
}

/******************************************************************************\
****  External Functions  ******************************************************
\******************************************************************************/

void init_engine(void) {
	SPRITES_8x16;
	SHOW_SPRITES;
	SHOW_BKG;

	set_tile_data(64, 64, font_tiles, 0x90);
}

// Supposedly a decent and fast 8-bit PRNG. Will need to see how it performs during gameplay, but until then I'll go on faith.
// Source: https://github.com/edrosten/8bit_rng/blob/master/rng-4261412736.c
uint8_t random(void) {
	uint8_t t = e_random_x ^ (e_random_x << 4);

	e_random_x = e_random_y;
	e_random_y = e_random_z;
	e_random_z = e_random_a;
	e_random_a = e_random_z ^ t ^ (e_random_z >> 1) ^ (t << 1);

	return e_random_a;
}

void render_text(uint8_t* const restrict tilemap,
	             const char* restrict string, uint16_t x, uint16_t y,
				 uint16_t max_width, uint16_t max_height,
				 bool intelligent_wrapping, uint8_t render_mode) {

    ASSERT(!intelligent_wrapping, "Intelligent wrapping is not yet supported.");

    uint16_t xoff = 0;
	uint16_t yoff = 0;

	for (; *string; string++) {
		set_vram_byte(tilemap + ((y+yoff)*BUFFER_WIDTH) + x+xoff, char_to_tile(64, *string));

		xoff += 1;
		if (xoff == max_width) {
			xoff = 0;
			yoff += 1;
		}
		if (yoff == max_height) {
			if (render_mode == TEXTMODE_NOSCROLL) break;
			if (render_mode == TEXTMODE_SCROLL) {
				waitpad(0xff);

				yoff -= 1;
				for (uint16_t cy = y; cy < (y+max_height); cy += 1) {
					for (uint16_t cx = x; cx < (x+max_width); cx += 1) {
						uint16_t addr = (cy*BUFFER_WIDTH) + cx;
						set_vram_byte(tilemap + addr, get_vram_byte(tilemap + addr + BUFFER_WIDTH));
					}
				}
				for (uint16_t cx = x; cx < (x+max_width); cx += 1) {
					set_vram_byte(tilemap + (y + max_height - 1)*BUFFER_WIDTH + cx, char_to_tile(64, ' '));
				}
			}
		}
		
		if (joypad()) {
			vsync();
		}
		else {
			for (uint8_t i = 0; i < e_text_speed; i++) vsync();
		}
	}
}
