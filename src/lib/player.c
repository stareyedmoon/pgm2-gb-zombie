#include <player.h>
#include <defines.h>

const PlayerArchetype player_archetype[] = {
	{10, 3, 3, 3},
	{15, 2, 4, 4},
	{ 7, 4, 2, 2},
	{ 6, 2, 2, 2},
};

Player player_new(char* name, uint8_t archetype) {
	Player player = {
		{0, 0, {255, 255}, 0, 0},
		{player_archetype[archetype].base_health,
		 player_archetype[archetype].base_health,
		 player_archetype[archetype].base_intelligence,
		 player_archetype[archetype].base_strength,
		 player_archetype[archetype].base_speed,
		 1, 1, 1, "PLAYER"},
		0,
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
	};

	return player;
}
