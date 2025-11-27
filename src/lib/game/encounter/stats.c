#include <stdint.h>
#include <stdbool.h>

#include <game/encounter.h>

#include <item.h>
#include <general.h>

EffectiveStats calculate_effective_stats(EncounterEntity* entity) {
    int16_t weight = weapon_item[entity->encounterable->weapon].weight
    + armor_item[entity->encounterable->armor].weight;
    
    int16_t intelligence = entity->encounterable->intelligence + entity->intelligence_effect;
    int16_t strength = entity->encounterable->strength + entity->strength_effect;
    int16_t speed = entity->encounterable->speed + entity->speed_effect;


    EffectiveStats stats = {
        CLAMP(intelligence, 0, 255),
        CLAMP(strength, 0, 255),
        CLAMP(speed - MAX(weight - strength, 0), 1, 255),
        false
    };

    stats.can_use_weapon =
           stats.strength >= weapon_item[entity->encounterable->weapon].strength_requirement
        && stats.intelligence >= weapon_item[entity->encounterable->weapon].intelligence_requirement;

    return stats;
}
