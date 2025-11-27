#include <stdint.h>

#include <game/encounter.h>

#include <item.h>
#include <general.h>

static uint8_t get_crit(EncounterEntity* attacker) {
    uint8_t crit_value = rand8();
    
    if (crit_value <= (attacker->effective_stats.intelligence / 85)) {
        return 10; 
    }
    else if (crit_value * 5 < attacker->effective_stats.speed) {
        return 6; 
    }
    else if (crit_value * 2 < attacker->effective_stats.speed) {
        return 3; 
    }
    else if (crit_value < attacker->effective_stats.speed) {
        return 2; 
    }
    else if (crit_value >= (252 + attacker->effective_stats.intelligence / 85)) {
        return 0;
    }
    else {
        return 1; 
    }
}

static uint16_t encode_damage_crit(uint16_t damage, uint8_t crit_multiplier) {
    switch (crit_multiplier) {
    case  0: damage = MIN(damage, 0x07FF) | 0x0000; break; // 00000 - miss
    case  1: damage = MIN(damage, 0x07FF) | 0x0800; break; // 00001 - normal
    case  2: damage = MIN(damage, 0x0FFF) | 0x1000; break; // 0001  - crit
    case  3: damage = MIN(damage, 0x1FFF) | 0x2000; break; // 001   - crit+
    case  6: damage = MIN(damage, 0x3FFF) | 0x4000; break; // 01    - extreme crit
    case 10: damage = MIN(damage, 0x7FFF) | 0x8000; break; // 1     - vital crit
    }

    return damage;
}


static uint16_t get_base_damage(EncounterEntity* attacker) {
    return attacker->effective_stats.strength
         + (attacker->effective_stats.can_use_weapon
            ? weapon_item[attacker->encounterable->weapon].damage
            : 0);
}

static uint16_t get_damage_random(void) {
    return simple_binom() + simple_binom() + simple_binom() + simple_binom() + simple_binom() + 215;
}

static uint8_t get_enemy_protection(EncounterEntity* attacker, EncounterEntity* target) {
    uint8_t damage_type = weapon_item[attacker->encounterable->weapon].damage_type;

    switch (damage_type) {
    case DAMAGE_TYPE_BLUNT:    return armor_item[target->encounterable->armor].blunt_protection;
    case DAMAGE_TYPE_PIERCING: return armor_item[target->encounterable->armor].piercing_protection;
    case DAMAGE_TYPE_STAB:     return armor_item[target->encounterable->armor].stab_protection;
    case DAMAGE_TYPE_SLASH:    return armor_item[target->encounterable->armor].slash_protection;
    default: return 0;
    }
}

uint16_t calculate_damage(EncounterEntity* attacker, EncounterEntity* target) {
    uint8_t crit_multiplier = get_crit(attacker);


    uint16_t base_damage = get_base_damage(attacker);

    // Adding 5 simple_binom together gets us the same result as running a binomial with 40 trials.
    uint8_t damage_random_factor = get_damage_random();

    uint16_t damage = base_damage;
    damage *= crit_multiplier;

    damage *= damage_random_factor;
    damage /= 255;

    damage *= 255 - get_enemy_protection(attacker, target);
    damage /= 255;

    damage = MAX(damage, 1);

    return encode_damage_crit(damage, crit_multiplier);;
}
