#include <stdint.h>

#ifndef _item_h_INCLUDE
#define _item_h_INCLUDE

// Items are divided into 4 blocks of 64 IDs each, 
// Not sure if I'm gonna keep the 'key' group, but for now I'll put it there.

#define ITEM_INDEX_BASE_KEY 0x00
#define ITEM_INDEX_BASE_CONSUMABLE 0x40
#define ITEM_INDEX_BASE_WEAPON 0x80
#define ITEM_INDEX_BASE_ARMOR 0xC0


#define DAMAGE_TYPE_BLUNT 0
#define DAMAGE_TYPE_PIERCING 1
#define DAMAGE_TYPE_STAB 2
#define DAMAGE_TYPE_SLASH 3


typedef struct {
    const char* name;
} KeyItem;

typedef struct {
    const char* name;

    // How much the item heals, or when negative, how much it hurts.
    int16_t healing_amount;

    // How many turns the timed effects of this item lasts.
    // When not actively in an encounter, effects are ignored.
    uint8_t effect_duration;

    // The timed effects of the item. During encounters, the stats of the consumer are changed by these values for however long the effect duration is.
    // You can't stack effects. If you try, the one already in effect will take precedent, unless it would expire this turn.

    int8_t effect_intelligence;
    int8_t effect_strength;
    int8_t effect_speed;
} ConsumableItem;

typedef struct {
    const char* name;
    const char* description;

    // Requirements to use the weapon. If either isn't met, then damage is ignored.

    uint8_t intelligence_requirement;
    uint8_t strength_requirement;
    
    // Weight of the weapon. Speed of the user is reduced by `max(0, weight - strength)`.
    uint8_t weight;

    // If the requirements are met, this is added to the base damage of any attack.
    uint8_t damage;

    // The damage type of the user is set to this, regardless of whether the requirements are met or not.
    uint8_t damage_type;
} WeaponItem;

typedef struct {
    const char* name;
    const char* description;

    // Weight of the armor. Speed of the wearer is reduced by `max(0, weight - strength)`.
    uint8_t weight;

    // How much this armor protects from each type of damage.
    // I haven't decided how the damage protection should be done yet, but I have a few ideas, so hopefully it won't take too long.

    uint8_t blunt_protection;
    uint8_t piercing_protection;
    uint8_t stab_protection;
    uint8_t slash_protection;
} ArmorItem;


extern const KeyItem key_item[];
extern const ConsumableItem consumable_item[];
extern const WeaponItem weapon_item[];
extern const ArmorItem armor_item[];


#endif // _item_h_INCLUDE
