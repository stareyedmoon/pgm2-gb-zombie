#include <stdint.h>

#ifndef _general_h_INCLUDE
#define _general_h_INCLUDE

/// @brief Generates a random 8-bit number.
uint8_t rand8(void);

/// @brief Flip all the bits in a byte.
uint8_t flip_byte(uint8_t value);

/// @brief Decompresses a sprite to the tile representation.
/// @param dest Pointer to the buffer to decompress to.
/// @param src Pointer to the compressed data.
/// @returns The size, in tiles, of the decompressed sprite. The high nibble is the width, and the low nibble is the height.
/// @warning As this function does not check buffer bounds for performance reasons, you MUST be careful that any data you decompress can actually fit in the destination.
uint8_t decompress_sprite(uint8_t* dest, uint8_t* src);

// Given that C doesn't have templates like C++ does, it's much easier to make macros than functions for each integer type.

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/// @brief Computes the absolute difference between two integers.
/// @note For signed integers this only works for numbers within INT_MAX of eachother.
#define DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

#define ABS(a) ((a) < 0 ? -(a) : (a))

/// @brief Computes subtraction with a minimum value of 0.
#define SATURATING_SUB(a, b) ((b) > (a) ? 0 : (a) - (b))

#endif // _general_h_INCLUDE
