// I really only ever need one instance of player at any one time, and hard-coding one at some
//   arbitrary location would probably be slightly better for performance, given we're on a
//   Gameboy, but the assignment wanted objects, and they're easier to work with, anyhow.

#include <stdint.h>
#include <stdbool.h>

#include <entity.h>

#ifndef _player_h_INCLUDE
#define _player_h_INCLUDE

/// @brief Maximum number of item types that can be stored in a players inventory.
#define PLAYER_INVENTORY_SIZE 8

/// @brief Maximum length of a player name in bytes.
#define PLAYER_NAME_LENGTH 8

/// @brief Generally normal person. Average health, intelligence, and strength.
#define PLAYER_ARCHETYPE_BASIC 0
/// @brief Somewhat stronger person. Above average health and strength, but lower intelligence (for balance purposes; I don't mean to imply that everyone who likes to be fit or strong is inherently stupider or anything like that).
#define PLAYER_ARCHETYPE_STRONGMAN 1
/// @brief More of a scientist or something, I guess. Lower health and strength (again, for balance), but higher intelligence (predictable enough).
#define PLAYER_ARCHETYPE_GOOD_THUNKER 2
/// @brief Quite a pathetic person. Below average in all respects. Just wanted a fourth to get to a power of two. Could be considered a rudementary "hard mode", I guess.
#define PLAYER_ARCHETYPE_PATHETIC 3

typedef struct {
    uint16_t base_health;
    uint8_t base_intelligence;
    uint8_t base_strength;
    uint8_t base_speed;
} PlayerArchetype;

extern const PlayerArchetype player_archetype[];

/// @brief Representation of a Player.
typedef struct {
    Entity entity;
    Encounterable encounterable;

    // When this reaches some threshold, the level (in Entity) of the player increases.
    // The threshold is determined by the level.
    uint16_t experience;

    // Item ID of all the items in the player's inventory. An ID of 0 represents an empty slot.
    uint8_t inventory_itemid[PLAYER_INVENTORY_SIZE];

    // How many items are stored in each slot of the inventory. Of course, it's ignored if the slot in question is empty.
    // Might add a bug intentionally (so, not really a bug) to duplicate items in some way.
    uint8_t inventory_count[PLAYER_INVENTORY_SIZE];
} Player;

/// @brief Creates a new player. Stats are decided by archetype, while Lv, Exp, inventory, weapon, and armor are initialized to 0/none.
/// @param name Name of the player. Just "Player" if NULL.
/// @param archetype Archetype of the player. Must match one of the archetypes defined near the top of this file.
/// @return Newly constructed player.
Player player_new(char* name, uint8_t archetype);

/// @brief Update the sprite for the player to point in the direction of movement.
/// @param player Pointer to the player to update the sprite of.
/// @param dx Delta X of movement, where negative is left and positive is right.
/// @param dy Delta Y of movement, where negative is up and positive is down.
/// @note The magnitude of dx and dy are ignored. Only their signs are used.
/// @note If both dx and dy are non-zero, then dx takes priority.
void player_update_sprite(Player* player, int8_t dx, int8_t dy);

/// @brief Attempts to add an item to the players inventory.
/// @param player Pointer to the player in question.
/// @param item ID of the item to add. Must be non-zero.
/// @param count How many of the item to add. Must be non-zero.
/// @return How many of the item were successfully added to the players inventory. A value of zero, therefore, represents complete failure.
uint8_t player_try_add_item(Player* player, uint8_t item, uint8_t count);

#endif // _player_h_INCLUDE
