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

Damage calculate_damage(EncounterEntity* attacker, EncounterEntity* target) {
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

    Damage dmg = {
	damage,
	crit_multiplier
    };
    return dmg;
}
