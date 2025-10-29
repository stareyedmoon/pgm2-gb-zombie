// I really only ever need one instance of player at any one time, and hard-coding one at some
//   arbitrary location would probably be slightly better for performance, given we're on a
//   Gameboy, but the assignment wanted objects, and they're easier to work with, anyhow.

#include <stdint.h>
#include <stdbool.h>

#ifndef _player_h_INCLUDE
#define _player_h_INCLUDE

/// @brief Maximum number of item types that can be stored in a players inventory.
#define PLAYER_INVENTORY_SIZE 8

/// @brief Maximum length of a player name in bytes.
#define PLAYER_NAME_LENGTH 9

/// @brief Generally normal person. Average health, intelligence, and strength.
#define PLAYER_ARCHETYPE_BASIC 0
/// @brief Somewhat stronger person. Above average health and strength, but lower intelligence (for balance purposes; I don't mean to imply that everyone who likes to be fit or strong is inherently stupider or anything like that).
#define PLAYER_ARCHETYPE_STRONGMAN 1
/// @brief More of a scientist or something, I guess. Lower health and strength (again, for balance), but higher intelligence (predictable enough).
#define PLAYER_ARCHETYPE_GOOD_THUNKER 2
/// @brief Quite a pathetic person. Below average in all respects. Just wanted a fourth to get to a power of two. Could be considered a rudementary "hard mode", I guess.
#define PLAYER_ARCHETYPE_PATHETIC 3

/// @brief Representation of a Player.
/// @note Takes 40 bytes of RAM per instance.
typedef struct {
    // x and y specify the players position in the overworld (where you move around).
    // They don't really do anything during battles.
    uint16_t x;
    uint16_t y;

    // If this reaches zero, you die. Pretty normal game stuff, you really should've been able to guess that.
    uint16_t health;

    // Influences damage calculations, and what weapons can be used, kinda like Dark Souls, except not at all.
    uint8_t intelligence;
    // Added to Player for gameplay purposes.
    // Influences damage calculations, and what weapons can be used (weapons can get quite heavy, you know).
    uint8_t strength;
    // Standard RPG stuff. Always nice to see a definite growth in stats over time. Just don't try to use a leveling system when writing a novel or something, it just doesn't fit there.
    // Influences what weapons can be used, and the growth of INT and STR. Again, kinda like Dark Souls.
    uint8_t level;
    // When this reaches some threshold determined by level, you level up.
    // Leveling up is good because it means you get stronger... and smarter? Murder making you smarter doesn't really work in the real world, but oh well.
    // I've never actually played Dark Souls, but I'm pretty sure it doesn't use an EXP-based system, and you instead buy levels using "souls", which are like the in-game currency? You get them from murder, at any rate. Makes sense I guess. Where else would you get someones soul, if not through murder?
    uint16_t experience;

    // Specifies which items are in the players inventory, where 0 represents none.
    uint8_t inventory_itemid[PLAYER_INVENTORY_SIZE];
    // Represents how many of each item type are in the players inventory. Ignored if item type is 0.
    // Might encode as BCD, depending on how expensive div-by-10 is. Would make the item limit 99 instead of 255, but I don't think that should be a significant limitation. Probably.
    uint8_t inventory_count[PLAYER_INVENTORY_SIZE];
    // Which item is being used as a weapon. Only weapon-type items can be used.
    // Influences damage calculations.
    uint8_t weapon;
    // Which item is being used as armor. Only armor-type items can be used.
    // Reduces damage taken, I'd imagine.
    uint8_t armor;

    // Player name. Doesn't really serve a gameplay purpose, but does it ever? (Well, I guess it does in some games, like for example Undertale.)
    char name[PLAYER_NAME_LENGTH];

    uint8_t sprite_index[2];
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
