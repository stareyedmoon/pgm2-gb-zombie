#include <stdint.h>

#ifndef _item_h_INCLUDE
#define _item_h_INCLUDE

// Items are divided into 4 blocks of 64 IDs each.
// Not sure if I'm gonna keep the 'key' group, but for now I'll put it there.
// If I remove the 'key' group, I might just expand other groups.

#define ITEM_INDEX_BASE_KEY 0x00
#define ITEM_INDEX_BASE_CONSUMABLE 0x40
#define ITEM_INDEX_BASE_WEAPON 0x80
#define ITEM_INDEX_BASE_ARMOR 0xC0

#define DAMAGE_TYPE_BLUNT 0
#define DAMAGE_TYPE_PIERCING 1
#define DAMAGE_TYPE_STAB 2
#define DAMAGE_TYPE_SLASH 3
#define DAMAGE_TYPE_DEFEND 4

typedef struct {
	const char* name; // Max 20 ch
	const char* description; // Max 40 ch
} KeyItem;

typedef struct {
	const char* name; // Max 20 ch
	const char* description; // Max 40 ch$

	// How much the item heals or, when negative, how much it hurts.
	int8_t healing_amount;

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
	const char* name; // Max 15 ch
	
	// The power of the attack. This, along with the base damage of the weapon, determines the weapon damage.
	uint8_t power;

	// The type of the attack. This can be STAB, SLASH, BLUNT, or PIERCING.
	uint8_t type;

	// How many turns it takes before this attack can be used again after use.
	uint8_t cooldown;

	// How much is subtracted from the turn counter when this attack is used.
	uint8_t time_cost;
} WeaponItemAttack;

typedef struct {
	const char* name; // Max 20 ch
	const char* description; // Max 40 ch

	// Requirements to use the weapon. If they aren't met, then damage is ignored.

	uint8_t intelligence_requirement;
	uint8_t strength_requirement;
    
	// Weight of the weapon. Speed of the user is reduced by `max(0, weight - strength)`.
	uint8_t weight;

	// Base damage of the weapon. This is modified by the attack power to get weapon damage.
	uint8_t damage;

	// The attacks the weapon can use. See WeaponItemAttack struct above for more information.
	WeaponItemAttack attacks[4];
} WeaponItem;

typedef struct {
	const char* name; // Max 20 ch
	const char* description; // Max 40 ch

	// Weight of the armor. Speed of the wearer is reduced by `max(0, weight - strength)`.
	uint8_t weight;

	// How much this armor protects from each type of damage.
	// Damage has multple "tiers". The first "tier" is 4 damage and below. Each subsequent tier is 4 times more than the previous.
	// For each tier an armour is less capable of absorbing its damage.
	// The formula for this is damage*(1-protection^p_n), where n is the tier, and p is [1, 2, 3, 4, 6, 9, 15 21].

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
