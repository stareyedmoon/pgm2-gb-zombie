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

#define COLLISION_LAYER_LEFT   0x01
#define COLLISION_LAYER_RIGHT  0x02
#define COLLISION_LAYER_UP     0x04
#define COLLISION_LAYER_DOWN   0x08
// 0x10
// 0x20
#define COLLISION_LAYER_ENTITY 0x40
#define COLLISION_LAYER_OOB    0x80

// Combination of _LEFT, _RIGHT, _UP, and _DOWN, preventing movement entirely.
#define COLLISION_LAYER_BLOCK  0x0F

extern uint8_t e_text_speed;


/// @brief Initialize all engine components 
void engine_init(void);

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
void engine_render_text(uint8_t* const restrict tilemap,
                 const char* restrict string,
				 uint16_t x,
				 uint16_t y,
				 uint16_t max_width,
				 uint16_t max_height,
				 bool intelligent_wrapping,
				 uint8_t render_mode);

/// @brief Set the active map to specified.
/// @param map Pointer to possibly-compressed map data.
void engine_set_map(const uint8_t* map);

/// @brief Allocates not-in-use sprite, and return its index.
/// @note Returns 255 if no sprite is available.
uint8_t engine_alloc_sprite(void);

/// @brief Free a sprite previously allocated by alloc_sprite.
/// @param sprite Index of the sprite to free.
void engine_free_sprite(uint8_t sprite);

/// @brief Move the camera centered at specified position, taking in to account world bounds. Loads in new parts of the map as needed.
void engine_move_camera(uint16_t x, uint16_t y);

/// @brief Move a sprite to some position, taking in to account the camera position.
/// @param sprite Index of the sprite to move.
void engine_move_sprite(uint8_t sprite, uint16_t x, uint16_t y);

/// @brief Get collision status of tile at specified coordinates.
/// @return A bitfield specifying which collision layers tile is active in.
uint8_t engine_get_collision(uint16_t x, uint16_t y);

#endif // _engine_h_INCLUDE
