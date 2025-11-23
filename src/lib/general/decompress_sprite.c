#include <stdint.h>
#include <stdbool.h>

#include <debug.h>

#include <gb/gb.h>


#define READ_BIT(src, off) (src[off/8]&(1<<(7-(off++%8)))?1:0)

static bool read_bit(uint8_t* src, uint16_t* offset) {
	bool value = src[*offset / 8] & (1 << (7 - *offset % 8));
	*offset += 1;
	return value;
}
// TODO - Optimize to use at most 2 reads instead of `count` reads.
//        Or one 16 bit read, I suppose.
static uint8_t read_bits(uint8_t* src, uint16_t* offset, uint8_t count) {
	ASSERT(count <= 8, "read_bits only supports up to 8 bits.");
	
	uint8_t value = 0;
	for (uint8_t i = 0; i < count; i++) {
		value <<= 1;
		value |= read_bit(src, offset) ? 1 : 0;
	}
	return value;
}


/************************** TODO **************************/
/* Reimplement compression algorithm in assembly.         */            
/* ------------------------------------------------------ */ 
/* The reason for doing that instead of optimising the C  */
/* code is that the compiler I'm using (SDCC/LCC) doesn't */
/* generate particuarly optimised code, at least for this */
/* target (sm83).                                         */
/*                                                        */
/* As an example, the line                                */
/* `uint8_t full_bars = MIN(fill / 8, 7)`                 */
/* in game/encounter.c compiles to 43 bytes, taking       */
/* 31 to 41 cycles to execute. Writing equivalent code    */
/* myself in assembly takes 15 bytes, taking 17 cycles to */
/* execute.                                               */
/*                                                        */
/* I could optimise the C code by writing code like       */
/* `uint8_t full_bars = fill/8;`                          */
/* `if (full_bars > 7) full_bars = 7;`,                   */
/* but that still doesn't generate code quite as fast as  */
/* assembly (taking 24 bytes and 25-26 cycles), so it     */
/* feels a bit pointless.                                 */
/**********************************************************/

#define X_TO_OFF(x) ((uint16_t)((x) & 0xF8) << 1)
#define Y_TO_OFF(y, width) (((uint16_t)((y) & 0x07) * 2) + (((uint16_t)((y) & 0xF8) << 1) * (uint16_t)(width)))

static void rldecode(uint8_t* dest, uint8_t* src, uint16_t* src_off, uint8_t width, uint8_t height) {
	uint8_t width_px = width * 8;
	uint8_t height_px = height * 8;


	bool packet_type = read_bit(src, src_off);

	uint16_t pending_zeroes = 0;
	for (uint8_t x = 0; x < width_px; x += 2) {
		uint8_t mask = 0xC0 >> (x & 0x06);
		uint8_t bitoff = 6 - (x & 0x06);

		uint16_t xoff = X_TO_OFF(x);

		if (pending_zeroes > height_px) {
			pending_zeroes -= height_px;
			continue;
		}

		for (uint8_t y = 0; y < height_px; y += 1) {
			uint16_t yoff = Y_TO_OFF(y, width);


			if (pending_zeroes) {
				pending_zeroes -= 1;
				continue;
			}

			if (packet_type) {
				// Data packet
				uint8_t data = read_bits(src, src_off, 2);
				if (data == 0) {
					packet_type = false;
				}
				else {
					set_vram_byte(dest + xoff + yoff, get_vram_byte(dest + xoff + yoff) | (data << bitoff));
					//set_vram_byte(dest + xoff + yoff, 0xff);

				}
			}
			if (!packet_type) {
				// RLE packet
				
				uint8_t magnitude = 1;
				while (read_bit(src, src_off)) magnitude++;
				uint16_t value = 1;
				for (; magnitude > 0; magnitude -= 1) {
					value <<= 1;
					value |= read_bit(src, src_off) ? 1 : 0;
				}

				pending_zeroes = value - 2;
				packet_type = true;
			}
		}
	}
}

extern void decompression_deltadecode(uint8_t* dest, uint8_t width, uint8_t height);

static void unxor(uint8_t* dest, uint8_t* src, uint8_t size) {
	for (uint16_t i = 0; i < (uint16_t)size * 16; i += 2) {
		// Hold on thats just an XOR. Why did I name this function `unxor`?
		set_vram_byte(dest + i, get_vram_byte(dest + i) ^ get_vram_byte(src + i));
	}
}

uint8_t decompress_sprite(uint8_t* dest, uint8_t* src) {
	uint16_t src_off = 0;

	uint8_t width = read_bits(src, &src_off, 4);
	uint8_t height = read_bits(src, &src_off, 4);

	for (uint16_t i = 0; i < (width*height*16); i += 1) {
		set_vram_byte(dest + i, 0);
	}

	bool swapped_buffers = read_bit(src, &src_off);

	rldecode(dest + swapped_buffers, src, &src_off, width, height);

	uint8_t mode = read_bit(src, &src_off);
	if (mode) mode = read_bit(src, &src_off) + 1;

	rldecode(dest + 1 - swapped_buffers, src, &src_off, width, height);

	decompression_deltadecode(dest + swapped_buffers, width, height);
	
	if (mode != 1) decompression_deltadecode(dest + 1 - swapped_buffers, width, height);

	if (mode) unxor(dest + 1 - swapped_buffers, dest + swapped_buffers, width * height);

	return (width << 4) | height;
}
