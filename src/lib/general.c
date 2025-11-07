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

#define READ_BIT(src, off) (src[off/8]&(1<<(7-(off++%8)))?1:0)

static bool read_bit(uint8_t* src, uint16_t* offset) {
	bool value = src[*offset / 8] & (1 << (7 - *offset % 8));
	*offset += 1;
	return value;
}
static uint8_t read_bits(uint8_t* src, uint16_t* offset, uint8_t count) {
	ASSERT(count <= 8, "read_bits only supports up to 8 bits.");
	
	uint8_t value = 0;
	for (uint8_t i = 0; i < count; i++) {
		value <<= 1;
		value |= read_bit(src, offset) ? 1 : 0;
	}
	return value;
}

static void rldecode(uint8_t* dest, uint8_t* src, uint16_t* src_off, uint8_t width, uint8_t height) {
	uint8_t width_px = width * 8;
	uint8_t height_px = height * 8;
}

uint8_t decompress_sprite(uint8_t* dest, uint8_t* src) {
	uint16_t src_off = 0;

	uint8_t width = read_bits(src, &src_off, 4);
	uint8_t height = read_bits(src, &src_off, 4);

	bool swapped_buffers = read_bit(src, &src_off);

	rldecode(dest + swapped_buffers?1:0, src, &src_off, width, height);

	uint8_t mode = read_bit(src, &src_off) ? 1 : 0;
	if (mode) mode = read_bit(src, &src_off) ? 2 : 1;

	rldecode(dest + swapped_buffers?0:1, src, &src_off, width, height);

	return (width << 4) | height;
}
