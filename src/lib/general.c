#include <stdint.h>

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
