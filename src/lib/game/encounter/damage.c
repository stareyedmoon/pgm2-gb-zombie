#include <stdint.h>

#include <game/encounter.h>

#include <item.h>
#include <general.h>

uint16_t calculate_damage(EncounterEntity* attacker, EncounterEntity* target) {
    uint8_t crit_multiplier = rand8();
    if (crit_multiplier <= (attacker->effective_stats.intelligence / 85)) crit_multiplier = 10;
    else if (crit_multiplier * 5 < attacker->effective_stats.speed) crit_multiplier = 6;
    else if (crit_multiplier * 2 < attacker->effective_stats.speed) crit_multiplier = 3;
    else if (crit_multiplier < attacker->effective_stats.speed) crit_multiplier = 2;
    else if (crit_multiplier <= (252 + attacker->effective_stats.intelligence / 85)) crit_multiplier = 0;
    else crit_multiplier = 1;

    uint8_t damage_type = weapon_item[attacker->encounterable->weapon].damage_type;
    uint16_t base_damage = attacker->effective_stats.strength
                         + attacker->effective_stats.can_use_weapon
                             ? weapon_item[attacker->encounterable->weapon].damage
                             : 0;

    // Adding 5 simple_binom together gets us the same result as running a binomial with 40 trials.
    uint8_t damage_modifier = simple_binom() + simple_binom() + simple_binom() + simple_binom() + simple_binom();

    uint16_t damage = base_damage * crit_multiplier * (damage_modifier + 215) / 255;




    switch (crit_multiplier) {
    case  0: damage = MAX(damage, 0x07FF) | 0x0000; break; // 00000 - miss
    case  1: damage = MAX(damage, 0x07FF) | 0x0800; break; // 00001 - normal
    case  2: damage = MAX(damage, 0x0FFF) | 0x1000; break; // 0001  - crit
    case  3: damage = MAX(damage, 0x1FFF) | 0x2000; break; // 001   - crit+
    case  6: damage = MAX(damage, 0x3FFF) | 0x4000; break; // 01    - extreme crit
    case 10: damage = MAX(damage, 0x7FFF) | 0x8000; break; // 1     - vital crit
    }

    return damage;
}
