#include <stdint.h>

// These aren't local to the function itself so that I'll be able to seed it, if needed.
static uint8_t g_random_x = 0;
static uint8_t g_random_y = 0;
static uint8_t g_random_z = 0;
static uint8_t g_random_a = 1;

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
	else if (val <= 8) return 1;
	else if (val <= 36) return 2;
	else if (val <= 92) return 3;
	else if (val <= 162) return 4;
	else if (val <= 218) return 5;
	else if (val <= 246) return 6;
	else if (val <= 254) return 7;
	else return 8;
}
