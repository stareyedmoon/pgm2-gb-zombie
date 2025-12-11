#include <general.h>
#include <stdint.h>

uint8_t linear_interp(uint8_t a, uint8_t b, uint8_t t) {
	uint16_t res = (uint16_t)a * (255 - t) + (uint16_t)b * t;
	return res / 255;
}

uint8_t log2(uint8_t val) {
	if (val == 0) return 0x80;

	uint8_t l2 = 8;

	if (val < 0x10) {
		l2 -= 4;
		val <<= 4;
	}
	if (val < 0x40) {
		l2 -= 2;
		val <<= 2;
	}
	if (val < 0x80) {
		l2 -= 1;
	}

	return l2;
}

uint8_t log2l(uint16_t val) {
	if (val == 0) return 0x80;

	uint8_t l2 = 16;

	if (val < 0x100) {
		l2 -= 8;
		val <<= 8;
	}
	if (val < 0x1000) {
		l2 -= 4;
		val <<= 4;
	}
	if (val < 0x4000) {
		l2 -= 2;
		val <<= 2;
	}
	if (val < 0x8000) {
		l2 -= 1;
	}

	return l2;
}

