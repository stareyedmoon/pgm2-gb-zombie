#include <stdint.h>

#ifndef _general_h_INCLUDE
#define _general_h_INCLUDE

/// @brief Generates a random 8-bit number.
uint8_t rand8(void);

/// @brief Flip all the bits in a byte.
uint8_t flip_byte(uint8_t value);


// 2b tiles. This is the default, and allows for four levels of brighness (or three for sprites).
#define TILE_FORMAT_2b 0
// 1b tiles. Takes half as many bits as 2b, but only allows for two levels of brightness.
#define TILE_FORMAT_1b 1
// Tiles are compressed with the same format as Pokemon Gen 1 sprites.
// This saves space, but does come at the cost of longer load times.
#define TILE_FORMAT_COMPRESSED 2

/// @brief Loads a tile in a format.
/// @param dest Pointer to the destination.
/// @param src Pointer to the source.
/// @param format Format bitmap of the source tile. Use TILE_FORMAT_*
//void load_tile(uint8_t* dest, uint8_t* src, uint8_t format);

/// @brief Loads some number of tiles in a format.
/// @param dest Pointer to the destination.
/// @param src Pointer to the source.
/// @param format Format bitmap of the source tiles. Use TILE_FORMAT_*
/// @param count Number of tiles to load.
//void load_tiles(uint8_t* dest, uint8_t* src, uint8_t format, uint8_t count);

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

#define ABS(a) (a < 0 ? -a : a)

#endif // _general_h_INCLUDE
