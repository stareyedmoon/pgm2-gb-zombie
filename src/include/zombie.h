#include <stdint.h>

#include <entity.h>

#ifndef _zombie_h_INCLUDE
#define _zombie_h_INCLUDE

/// @brief Maximum length of a zombie name in bytes.
#define ZOMBIE_NAME_LENGTH 9

/// @brief Representation of a Zombie.
/// @note Takes 17 bytes. (Not ideal, in my opinion. Will think about this later, though.)
typedef struct {
    Entity entity;
    Encounterable encounterable;

    // The zombie type, which is needed because apparently they can evolve and whatever.
    // Determines appearance, name, and general stats (at creation).
    // This won't actually ever change, since (if I were to implement it) zombies could only level up and evolve if they won a battle, which could only happen if the player *lost* a battle, and if that happened the game would be over anyway.
    uint8_t type;
} Zombie;

/// @brief Creates a new zombie, deciding health, strength, level, weapon, and armor randomly, influenced by type and difficulty.
/// @param name Name of the zombie. Uses type name if NULL.
/// @param type Type of the zombie.
/// @param difficulty General "difficulty" of the zombie. Higher means better weapons, armor, and stats.
/// @return Newly constructed zombie.
Zombie zombie_new(char* name, uint8_t type, uint8_t difficulty);

#endif _zombie_h_INCLUDE
