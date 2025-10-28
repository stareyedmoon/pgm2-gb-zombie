#include <stdint.h>

#ifndef _zombie_h_INCLUDE
#define _zombie_h_INCLUDE

/// @brief Maximum length of a zombie name in bytes.
#define ZOMBIE_NAME_LENGTH 9

/// @brief Representation of a Zombie.
/// @note Takes 16 bytes of RAM per instance.
typedef struct {
    // x and y are not necessary as zombies do not appear as entities in the
    // overworld, probably. Plus, it saves 4 bytes of RAM.

    // These three fields have the same meaning as they do for Player.
    uint16_t health;
    uint8_t strength;
    uint8_t level;

    // Zombies don't need experience, as they are created as needed when entering a battle.
    // This does mean I don't have any need for an 'evolve' function, but I don't think such a function would make terribly much sense anyhow, given that they would only evolve when you've already died.

    // The type of the zombie. Gotta have one, since apparently this is Pokémon (you know, with the evolution).
    // This is decided through RNG affected by your location. They should get stronger the further you are, after all.
    uint8_t type;

    // Again, same as for Player.
    uint8_t weapon;
    uint8_t armor;


    // Probably just contains the name of the zombie type.
    // Might have a special name for some zombies, if I can be bothered.
    char name[ZOMBIE_NAME_LENGTH];
} Zombie;

/// @brief Creates a new zombie, deciding health, strength, level, weapon, and armor randomly, influenced by type and difficulty.
/// @param name Name of the zombie. Uses type name if NULL.
/// @param type Type of the zombie.
/// @param difficulty General "difficulty" of the zombie. Higher means better weapons, armor, and stats.
/// @return Newly constructed zombie.
Zombie zombie_new(char* name, uint8_t type, uint8_t difficulty);

#endif _zombie_h_INCLUDE
