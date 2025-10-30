#include <gb/gb.h>

#include <engine.h>
#include <general.h>
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
static uint16_t e_map_width = 0;
static uint16_t e_map_height = 0;

static uint16_t e_map_loaded_left = 0;
static uint16_t e_map_loaded_up = 0;

static uint16_t e_map_loaded_right = 31;
static uint16_t e_map_loaded_down = 31;

static uint16_t e_camera_x = 0;
static uint16_t e_camera_y = 0;

// Need to store X and Y for each sprite so that moving the camera will work.
static uint16_t e_sprite_x[MAX_HARDWARE_SPRITES];
static uint16_t e_sprite_y[MAX_HARDWARE_SPRITES];
// Might replace with a bitmap, if RAM runs real low.
// If it was large (say, >=512 elements), I'd probably use a binary allocation tree. (Don't know the actual name, that's just what I call it.)
static bool e_sprite_active[MAX_HARDWARE_SPRITES];

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

	ASSERT(0, "Invalid character conversion.");
	return base_tile + 0x1F;
}

/// @brief Updates sprite position for specified sprite. Used when either sprites or the camera moves.
/// @param sprite Index of the sprite to update.
void update_sprite(uint8_t sprite) {
	ASSERT(sprite < MAX_HARDWARE_SPRITES, "Tried to update invalid sprite.");

	if (DIFF(e_sprite_x[sprite], e_camera_x) < SCREEN_WIDTH_2 &&
	    DIFF(e_sprite_y[sprite], e_camera_y) < SCREEN_HEIGHT_2) {
		uint8_t posx = (e_sprite_x[sprite] + SCREEN_WIDTH_2 - e_camera_x) * 8 + (DEVICE_SPRITE_PX_OFFSET_X/2);
		uint8_t posy = (e_sprite_y[sprite] + SCREEN_WIDTH_2 - e_camera_y) * 8 + (DEVICE_SPRITE_PX_OFFSET_Y/2);

		move_sprite(sprite, posx, posy);
	}
	else {
		move_sprite(sprite, 0, 0);
	}
}

/******************************************************************************\
****  External Functions  ******************************************************
\******************************************************************************/

void engine_init(void) {
	SPRITES_8x16;
	SHOW_SPRITES;
	SHOW_BKG;

	set_tile_data(64, 64, font_tiles, 0x90);

	e_map = NULL;

	for (uint8_t i = 0; i < MAX_HARDWARE_SPRITES; i += 1) {
		e_sprite_x[i] = 0;
		e_sprite_y[i] = 0;
		e_sprite_active[i] = false;
	}
}

void engine_render_text(uint8_t* const restrict tilemap,
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

void engine_set_map(const uint8_t* map) {
	e_map = map + 4;
	e_map_width  = ((uint16_t)map[1] << 8) | (uint16_t)map[0];
	e_map_height = ((uint16_t)map[3] << 8) | (uint16_t)map[2];

	load_map(TILEMAP0, e_map, e_map_width, e_map_height, 0, 0, MIN(BUFFER_WIDTH, e_map_width), MIN(BUFFER_HEIGHT, e_map_height));
}

uint8_t engine_alloc_sprite(void) {
	for (uint8_t i = 0; i < MAX_HARDWARE_SPRITES; i += 1) {
		if (!e_sprite_active[i]) {
			e_sprite_active[i] = true;
			return i;
		}
	}

	return 255;
}

void engine_free_sprite(uint8_t sprite) {
	ASSERT(sprite < MAX_HARDWARE_SPRITES, "Attempted to free invalid sprite index.");
	ASSERT(e_sprite_active[sprite], "Attempted to free not-in-use sprite.");

	e_sprite_active[sprite] = false;

	move_sprite(sprite, 0, 0);
}

void engine_move_camera(uint16_t x, uint16_t y) {
	// TODO - Check for off-by-ones
	if (x < SCREEN_WIDTH_2) x = SCREEN_WIDTH_2;
	if (y < SCREEN_HEIGHT_2) y = SCREEN_HEIGHT_2;
	if (x > (e_map_width - SCREEN_WIDTH_2)) x = e_map_width - SCREEN_WIDTH_2;
	if (y < (e_map_height - SCREEN_HEIGHT_2)) y = e_map_height - SCREEN_HEIGHT_2;

	int16_t diff_x = DIFF((int16_t)e_camera_x, (int16_t)x);
	int16_t diff_y = DIFF((int16_t)e_camera_y, (int16_t)y);

	if (e_map_width <= BUFFER_WIDTH) diff_x = 0;
	if (e_map_height <= BUFFER_HEIGHT) diff_y = 0;

	// This code probably doesn't work.
	if (diff_x) {
		load_map(
			TILEMAP0,
			e_map,
			e_map_width,
			e_map_height,
			diff_x > 0 ? e_map_loaded_right : e_map_loaded_left + diff_x,
			MAX(e_map_loaded_up, e_map_loaded_up + diff_y),
			ABS(diff_x),
			BUFFER_HEIGHT - ABS(diff_y)
		);

		e_map_loaded_left += diff_x;
		e_map_loaded_right += diff_x;
	}
	if (diff_y) {
		load_map(
			TILEMAP0,
			e_map,
			e_map_width,
			e_map_height,
			e_map_loaded_left,
			diff_y > 0 ? e_map_loaded_down : e_map_loaded_up + diff_y,
			BUFFER_WIDTH,
			ABS(diff_y)
		);

		e_map_loaded_up += diff_y;
		e_map_loaded_down += diff_y;
	}
}

void engine_move_sprite(uint8_t sprite, uint16_t x, uint16_t y) {
	ASSERT(sprite < MAX_HARDWARE_SPRITES, "Tried to move invalid sprite.");
	ASSERT(e_sprite_active[sprite], "Tried to move inactive sprite.");

	e_sprite_x[sprite] = x;
	e_sprite_y[sprite] = y;

	update_sprite(sprite);
}

uint8_t engine_get_collision(uint16_t x, uint16_t y) {
	if (x >= e_map_width) return COLLISION_LAYER_OOB;
	if (y >= e_map_height) return COLLISION_LAYER_OOB;

	if (e_map[y*e_map_width + x] & 0x20) return COLLISION_LAYER_BLOCK;
	return 0;
}
