#include <gb/gb.h>

#include <engine.h>
#include <defines.h>

#include <debug.h>


static uint8_t e_text_speed = 0;


void load_map(uint8_t* const restrict dest, const uint8_t* const restrict src,
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

void set_text_speed(uint8_t speed) {
	e_text_speed = speed;
}

uint8_t get_text_speed(void) {
	return e_text_speed;
}

int8_t char_to_tile(uint8_t base_tile, char c) {
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
