#include <item.h>

const KeyItem key_item[] = {
	{ "NONE" },
};

const ConsumableItem consumable_item[] = {
	{ "NONE",
		"THERE'S NOTHING THERE",

		0,

		0,
		0, 0, 0
	}
};

const WeaponItem weapon_item[] = {
	{ "FISTS",
		"CAN'T OUTLAW FISTS",

		0, 0,

		0,

		3,

		{
			{
				"PUNCH",
				192,
				DAMAGE_TYPE_BLUNT,
				2,
				128
			},
			{
				"FAST PUNCH",
				128,
				DAMAGE_TYPE_BLUNT,
				1,
				100
			},
			{
				"THNDR CRSS SPLT",
				255,
				DAMAGE_TYPE_BLUNT,
				4,
				255
			},
			{
				"DEFEND",
				0,
				DAMAGE_TYPE_DEFEND,
				1,
				64
			}
		}
	}
};

const ArmorItem armor_item[] = {
	{ "CLOTHES",
		"THEY WILL NOT PROTECT YOU from me",

		0,

		0, 0, 0, 0
	}
};

