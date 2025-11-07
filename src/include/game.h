#include <entity.h>
#include <player.h>
#include <zombie.h>

#ifndef _game_h_INCLUDE
#define _game_h_INCLUDE

void game_main(void);

void game_encounter(Encounterable* player, Encounterable* enemy, uint8_t* enemy_sprite);

#endif // _game_h_INCLUDE
