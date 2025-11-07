#include <zombie.h>


const ZombieType zombie_type[] = {
	{"JON BIGRM", 18, 8, 4},
};

Zombie zombie_new(uint8_t type, uint8_t difficulty) {
	Zombie zombie = {
		{0, 0, {255, 255}, 0, 0},
		{zombie_type[type].base_health,
		 0,
		 zombie_type[type].base_strength,
		 zombie_type[type].base_speed,
		 difficulty, 0, 0, 0},
		type
	};

	return zombie;
}
