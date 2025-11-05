#include <item.h>

const KeyItem key_item[] = {

};

const ConsumableItem consumable_item[] = {
	ConsumableItem{"COFFEE",
		-1, /* Healing amount */

		 2, /* Effect duration */
		 1, /* Intelligence effect */
		 1, /* Strength effect */
		 1  /* Speed effect */
	},
	ConsumableItem{"INDISTINCT DRUGS",
		-10, /* Healing amount */

		  8, /* Effect duration */
		- 5, /* Intelligence effect */
		  2, /* Strength effect */
		  2  /* Speed effect */
	},

	ConsumableItem{"BEANS",
		30, /* Healing amount */

		 1, /* Effect duration */
		 0, /* Intelligence effect */
		-1, /* Strength effect */
		-1  /* Speed effect */
	},

	ConsumableItem{"magic SUPER JUICE",
		127, /* Healing amount */

		255, /* Effect duration */
		127, /* Intelligence effect */
		127, /* Strength effect */
		127  /* Speed effect */
	},
};

// I imagine I'll be adjusting all the requirements and such later.
// Possible changes to make:
// - Ranged weapons have higher damage at the cost of taking ammo?
// - Ranged weapons don't get extra damage (from buffs, dmg, etc)?
// TODO:
// - Redo all the requirement, weight, and damage numbers
const WeaponItem weapon_item[] = {
	WeaponItem{"FISTS",
		"CAN'T OUTLAW FISTS.",
		0, /* Intelligence requirement */
		0, /* Strength requirement */
		0, /* Weight */
		0, /* Damage */
		DAMAGE_TYPE_BLUNT
	},
	WeaponItem{"BRASS KNUCKLES",
		"THEY'RE IN YOUR\nBACK POCKET.",
		0, /* Intelligence requirement */
		0, /* Strength requirement */
		0, /* Weight */
		1, /* Damage */
		DAMAGE_TYPE_BLUNT
	},
	WeaponItem{"DOOM FISTS",
		"RIP AND TEAR.",
		  0, /* Intelligence requirement */
		255, /* Strength requirement */
		255, /* Weight */
		255, /* Damage */
		DAMAGE_TYPE_BLUNT
	},

	// Club series
	WeaponItem{"THICK STICK",
		"NOT TERRIBLY\nEFFECTIVE.",
		0, /* Intelligence requirement */
		0, /* Strength requirement */
		1, /* Weight */
		2, /* Damage */
		DAMAGE_TYPE_BLUNT
	},
	WeaponItem{"CLUB",
		"",
		0, /* Intelligence requirement */
		2, /* Strength requirement */
		3, /* Weight */
		5, /* Damage */
		DAMAGE_TYPE_BLUNT
	},
	WeaponItem{"IRON BASEBALL BAT",
		"OUCH.",
		0, /* Intelligence requirement */
		2, /* Strength requirement */
		3, /* Weight */
		5, /* Damage */
		DAMAGE_TYPE_BLUNT
	},
	WeaponItem{"MACE",
		"",
		0, /* Intelligence requirement */
		2, /* Strength requirement */
		3, /* Weight */
		5, /* Damage */
		DAMAGE_TYPE_BLUNT
	},
	WeaponItem{"COMICLY OVRSZD SPOON",
		"IT'S ABOUT seventy\nkilometers TALL.",
		  5, /* Intelligence requirement */
		250, /* Strength requirement */
		  1, /* Weight */
		255, /* Damage */
		DAMAGE_TYPE_BLUNT
	},

	// Knife series
	WeaponItem{"EATING KNIFE",
		"WHAT'S FOR DINNER?\nit's you.",
		1, /* Intelligence requirement */
		0, /* Strength requirement */
		0, /* Weight */
		3, /* Damage */
		DAMAGE_TYPE_STAB
	},
	WeaponItem{"DULL KNIFE",
		"",
		1, /* Intelligence requirement */
		2, /* Strength requirement */
		1, /* Weight */
		4, /* Damage */
		DAMAGE_TYPE_STAB
	},
	WeaponItem{"KITCHEN KNIFE",
		"WHERE'S THE\nchocolate?",
		2, /* Intelligence requirement */
		1, /* Strength requirement */
		1, /* Weight */
		7, /* Damage */
		DAMAGE_TYPE_STAB
	},
	WeaponItem{"STANDARD SWTCH KNIFE",
		"THE foundation WON'TMISS IT.",
		230, /* Intelligence requirement */
		 25, /* Strength requirement */
		  2, /* Weight */
		255, /* Damage */
		DAMAGE_TYPE_STAB
	},

	// Shortsword series
	WeaponItem{"BROKEN SHORTSWORD",
		"EXTRA SHORT.",
		 9, /* Intelligence requirement */
		 4, /* Strength requirement */
		 5, /* Weight */
		 8, /* Damage */
		DAMAGE_TYPE_SLASH
	},
	WeaponItem{"REPAIRED SHORTSWORD",
		"QUITE BLUNT.",
		23, /* Intelligence requirement */
		12, /* Strength requirement */
		11, /* Weight */
		17, /* Damage */
		DAMAGE_TYPE_SLASH
	},
	WeaponItem{"SHORTSWORD",
		"IT'S bronze FOR SOMEREASON.",
		29, /* Intelligence requirement */
		19, /* Strength requirement */
		24, /* Weight */
		28, /* Damage */
		DAMAGE_TYPE_SLASH
	},
	WeaponItem{"STEEL SHORTSWORD",
		"BETTER THAN bronze.",
		47, /* Intelligence requirement */
		27, /* Strength requirement */
		50, /* Weight */
		55, /* Damage */
		DAMAGE_TYPE_SLASH
	},
	WeaponItem{"TUNGSTEN SHORTSWORD",
		"HOW DURABLE IS THIS\nTHING, ANYWAY?",
		 68, /* Intelligence requirement */
		 35, /* Strength requirement */
		 80, /* Weight */
		100, /* Damage */
		DAMAGE_TYPE_SLASH
	},
	WeaponItem{"sefirot'S SHORTSWORD",
		"ACTUALLY QUITE LONG.",
		180, /* Intelligence requirement */
		 75, /* Strength requirement */
		140, /* Weight */
		155, /* Damage */
		DAMAGE_TYPE_SLASH
	},

	// Arrow series
	WeaponItem{"CLASSIC BOW",
		"DON'T ASK WHERE THE\nARROWS COME FROM.",
		10, /* Intelligence requirement */
		 5, /* Strength requirement */
		 3, /* Weight */
		12, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"LONGBOW",
		"NOT FOR USE WITH A\nCELLO.",
		13, /* Intelligence requirement */
		 7, /* Strength requirement */
		 5, /* Weight */
		21, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"COMPOUND BOW",
		"IT'S A BOW MADE OUT\nOF COMPOUNDS.",
		18, /* Intelligence requirement */
		13, /* Strength requirement */
		 9, /* Weight */
		32, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"CROSSBOW",
		"GOOD FOR SHOOTING A\nhamon user.",
		25, /* Intelligence requirement */
		19, /* Strength requirement */
		15, /* Weight */
		58, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"REPEATER",
		"THESE ARE STILL\nBEING MADE?",
		40, /* Intelligence requirement */
		24, /* Strength requirement */
		23, /* Weight */
		92, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"LARGE CROSSBOW",
		"GOOD FOR SHOOTING ANatmospheric rift.",
		 63, /* Intelligence requirement */
		 40, /* Strength requirement */
		 37, /* Weight */
		150, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"eventide",
		"LIKE BEING SHOT BY Arainbow.",
		163, /* Intelligence requirement */
		 92, /* Strength requirement */
		120, /* Weight */
		255, /* Damage */
		DAMAGE_TYPE_PIERCING
	},

	// Gun series
	WeaponItem{"PISTOL",
		"THE WEIGHT IS MOSTLYSPIRITUAL.",
		 0, /* Intelligence requirement */
		 0, /* Strength requirement */
		30, /* Weight */
		20, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
	WeaponItem{"FACTORY GUN",
		"THE bullets ARE MADEOUT OF teeth.",
		 65, /* Intelligence requirement */
		190, /* Strength requirement */
		150, /* Weight */
		255, /* Damage */
		DAMAGE_TYPE_PIERCING
	},
};

const ArmorItem armor_item[] = {
	ArmorItem{"CLOTHES",
		"NOT MUCH FOR PROTECTION.",

		0, /* Weight */

		0, /* Blunt protection */
		0, /* Piercing protection */
		1, /* Stab protection */
		1  /* Slash protection */
	},
	ArmorItem{"LIGHT BODYARMOR",
		"DON'T WORRY, YOU\nSTILL HAVE CLOTHES.",

		4, /* Weight */

		3, /* Blunt protection */
		2, /* Piercing protection */
		3, /* Stab protection */
		6  /* Slash protection */
	},
	ArmorItem{"CHAINMAIL",
		"DON'T GET SHOT :)",

		6, /* Weight */

		3, /* Blunt protection */
		0, /* Piercing protection */
		5, /* Stab protection */
		8  /* Slash protection */
	},
	ArmorItem{"FULLPLATE ARMOR",
		"A LITTLE HEAVY.",

		40, /* Weight */

		60, /* Blunt protection */
		80, /* Piercing protection */
		90, /* Stab protection */
		90  /* Slash protection */
	},
};
