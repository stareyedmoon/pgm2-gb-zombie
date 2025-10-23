#include <stdint.h>
#include <stdbool.h>

#ifndef _engine_h_INCLUDE
#define _engine_h_INCLUDE

/// @brief Load mapdata into tilemap
/// @param dest Pointer to tilemap to render into
/// @param src Pointer to map data to load from
/// @param map_width Width of map data
/// @param map_height Height of map data
/// @param x Left edge of map data to load
/// @param y Top edge of map data to load
/// @param w Width of map data to load
/// @param h Height of map data to load
void load_map(uint8_t* const restrict dest,
	          const uint8_t* const restrict src,
			  uint16_t map_width,
			  uint16_t map_height,
			  uint16_t x,
			  uint16_t y,
			  uint16_t w,
			  uint16_t h);

// Render text without scrolling.
// The behaviour when text can't fit in the specified box is undefined.
#define TEXTMODE_NOSCROLL 0
// Render text with scrolling.
// When text cannot fit in the specified box, split text into chunks and go to the next chunk on input
#define TEXTMODE_SCROLL 1

/// @brief Set text speed for character-by-character text rendering using `render_text`.
/// @param speed Time to display each character in frames. A speed of 0 will render instantly.
/// @bug A speed of 255 will freeze indefinitely.
void set_text_speed(uint8_t speed);

/// @brief Get text speed used by `render_text` for character-by-character rendering.
/// @return Time to display each character in frames.
uint8_t get_text_speed(void);

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
