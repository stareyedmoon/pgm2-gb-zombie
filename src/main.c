#include <gb/gb.h>

#include <stdint.h>

#include <engine.h>
#include <player.h>
#include <zombie.h>
#include <game.h>
#include <general.h>

#include <resource/zombie.h>

void main(void) {
    engine_init();

    Player player = player_new(NULL, PLAYER_ARCHETYPE_STRONGMAN);
    Zombie zombie = zombie_new(ZOMBIE_TYPE_JONNY_BIGARM, 0);


    game_encounter(&player.encounterable, &zombie.encounterable, zombie_data);
}
