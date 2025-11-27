#include <stdint.h>

#ifndef _entity_h_INCLUDE
#define _entity_h_INCLUDE

/// @brief Contains minimum required information for an entity.
/// @note Takes 8 bytes.
typedef struct {
    // The entity's position in the overworld.
    uint16_t x;
    uint16_t y;

    // The two sprite indicies used by the entity to display itself in the overworld.
    uint8_t sprite_index[2];

    // The base tile index of the entity's sprite sheet.
    // The sprite sheet contains frames for standing and walking in each direction.
    uint8_t base_tile_index;

    // Padding to bring the struct to 8 bytes (a power of 2). I'll try to figure out something to put here eventually.
    uint8_t padding;
} Entity;

/// @brief Contains all battle-specific entity information.
/// @note Takes 8 bytes.
typedef struct {
    // The maximum health of the encounterable. The health of the encounterable cannot go higher than this.
    uint8_t max_health;

    // Health of the encounterable, which for the purposes of simplifying code, includes the player. If it reaches zero, it dies.
    uint8_t health;

    // The INT stat of the encounterable.
    // This has a slight effect on damage, and limits what weapons can be used.
    // For zombies this is always zero, since zombies are mostly depicted of not being capable of much of any thought.
    uint8_t intelligence;

    // The STR stat of the encounterable.
    // This has a moderate effect on damage, and limits what weapons can be used.
    // Often higher for zombies, since their brains wouldn't be there to limit them, like it does for us mere mortals.
    uint8_t strength;

    // The AGI stat of the encounterable.
    // Doesn't have any effect on damage or what weapons can be used, and instead determines turn order. Essentially, higher speeds will let you have a turn more often than your enemy.
    // I'm not sure how this is gonna be balanced between the player and the zombies yet.
    uint8_t speed;

    // The level of the encounterable.
    // Doesn't have an effect on its own, but for zombies it decides how high the other stats are and what equipment it has, while for the player it decides how much experience they need to level up (which increases their stats).
    uint8_t level;

    // The weapon of the encounterable.
    // For an encounterable that has the necessary stats to use it, the weapon will increase the amount of damage done, perhaps at the cost of speed if it's heavy.
    // For encounterables that don't have the stats, it'll do nothing but slow them down and dull their attacks.
    // Whether you have the stats or not though, it will still decide what damage type you do.
    uint8_t weapon;

    // The armor of the encounterable.
    // Heavy armor reduces speed, which is counteracted by strength.
    // Different armors absorb different damage types to different extents, not taking account the stats of the encounterable.
    uint8_t armor;

    // Name of the encounterable.
    // Mainly used to show the name of the enemy in encounters, but is probably gonna be used for some other things too.
    const char* name;
} Encounterable;

#endif // _entity_h_INCLUDE
