#include <stdint.h>
#include <stdbool.h>

#include <player.h>

#ifndef _engine_h_INCLUDE
#define _engine_h_INCLUDE

// Render text without scrolling.
// The behaviour when text can't fit in the specified box is undefined.
#define TEXTMODE_NOSCROLL 0
// Render text with scrolling.
// When text cannot fit in the specified box, split text into chunks and go to the next chunk on input
#define TEXTMODE_SCROLL 1


extern uint8_t e_text_speed;


/// @brief Initialize all engine components 
void init_engine(void);

/// @brief Generates a random 8-bit number.
/// @return Should be rather obvious, I hope.
uint8_t random(void);

/// @brief Render text onto the target tilemap
/// @param tilemap Pointer to tilemap to render text onto
/// @param string ASCII-encoded string to write
/// @param x X-coordinate relative to the top-left of the tilemap to render the text to
/// @param y Y-coordinate relative to the top-left of the tilemap to render the text to
/// @param max_width Maximum width of a single line of text in characters
/// @param max_height Maximum height of the text in characters
/// @param intelligent_wrapping Wrap at nearest breaking character instead of at width
/// @param render_mode Specifies which way to render the text. Mode identifiers are prefixed with `TEXTMODE_`
/// @note Assumes characters are loaded at tiles 64-127
void render_text(uint8_t* const restrict tilemap,
                 const char* restrict string,
				 uint16_t x,
				 uint16_t y,
				 uint16_t max_width,
				 uint16_t max_height,
				 bool intelligent_wrapping,
				 uint8_t render_mode);

#endif // _engine_h_INCLUDE
