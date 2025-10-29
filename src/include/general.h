#include <stdint.h>

#ifndef _general_h_INCLUDE
#define _general_h_INCLUDE

/// @brief Generates a random 8-bit number.
uint8_t rand8(void);

// Given that C doesn't have templates like C++ does, it's much easier to make macros than functions for each integer type.

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/// @brief Computes the absolute difference between two integers.
/// @note For signed integers this only works for numbers within INT_MAX of eachother.
#define DIFF(a, b) ((a) > (b) ? (a) - (b) : (b) - (a))

#define ABS(a) (a < 0 ? -a : a)

#endif // _general_h_INCLUDE
