#include <stdint.h>
#include <stdbool.h>

#include <debug.h>

#include <gb/gb.h>

// These aren't local to the function itself so that I'll be able to seed it, if needed.
static uint8_t g_random_x = 0;
static uint8_t g_random_y = 0;
static uint8_t g_random_z = 0;
static uint8_t g_random_a = 1;

// Supposedly a decent and fast 8-bit PRNG. Will need to see how it performs during gameplay, but until then I'll go on faith.
// Source: https://github.com/edrosten/8bit_rng/blob/master/rng-4261412736.c
uint8_t rand8(void) {
	uint8_t t = g_random_x ^ (g_random_x << 4);

	g_random_x = g_random_y;
	g_random_y = g_random_z;
	g_random_z = g_random_a;
	g_random_a = g_random_z ^ t ^ (g_random_z >> 1) ^ (t << 1);

	return g_random_a;
}

uint8_t simple_binom(void) {
	uint8_t val = rand8();

	if (val <= 0) return 0;
	if (val <= 8) return 1;
	if (val <= 36) return 2;
	if (val <= 92) return 3;
	if (val <= 162) return 4;
	if (val <= 218) return 5;
	if (val <= 246) return 6;
	if (val <= 254) return 7;
	if (val <= 255) return 8;
}

uint8_t flip_byte(uint8_t value) {
	static const uint8_t flip_lut[16] = {
		0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF
	};

	return (flip_lut[value & 0x0F] << 4) | flip_lut[value >> 4];
}

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

static void deltadecode(uint8_t* dest, uint8_t width, uint8_t height) {
	uint8_t width_px = width * 8;
	uint8_t height_px = height * 8;

	for (uint8_t y = 0; y < height_px; y += 1) {
		uint16_t yoff = Y_TO_OFF(y, width);

		bool prev = false;
		for (uint8_t x = 0; x < width_px; x += 1) {
			uint16_t xoff = X_TO_OFF(x);
			uint8_t mask = 0x80 >> (x & 0x07);

			uint8_t value = get_vram_byte(dest + xoff + yoff);
			if (value & mask) prev = !prev;

			if (prev) set_vram_byte(dest + xoff + yoff, value | mask);
			else set_vram_byte(dest + xoff + yoff, value & ~mask);
		}
	}
}

static void unxor(uint8_t* dest, uint8_t* src, uint8_t size) {
	for (uint16_t i = 0; i < (uint16_t)size * 16; i += 2) {
		set_vram_byte(dest + i, get_vram_byte(dest + i) ^ get_vram_byte(src + i));
	}
}

uint8_t decompress_sprite(uint8_t* dest, uint8_t* src) {
	uint16_t src_off = 0;

	uint8_t width = read_bits(src, &src_off, 4);
	uint8_t height = read_bits(src, &src_off, 4);

	bool swapped_buffers = read_bit(src, &src_off);

	rldecode(dest + swapped_buffers, src, &src_off, width, height);

	uint8_t mode = read_bit(src, &src_off);
	if (mode) mode = read_bit(src, &src_off) + 1;

	rldecode(dest + 1 - swapped_buffers, src, &src_off, width, height);

	deltadecode(dest + swapped_buffers, width, height);

	if (mode != 1) deltadecode(dest + 1 - swapped_buffers, width, height);

	if (mode) unxor(dest + 1 - swapped_buffers, dest + swapped_buffers, width * height);

	return (width << 4) | height;
}
