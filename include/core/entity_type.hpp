#ifndef CORE_ENTITY_TYPE_HPP
#define CORE_ENTITY_TYPE_HPP

namespace core {

    enum class EntityType {
        ABSTRACT_ENTITY,
        ABSTRACT_BLOCK,
        ABSTRACT_MOB,
        ABSTRACT_COLLECTIBLE,
        PLAYER_BULLET,
        WALL,
        AIR,
        PLAYER,
        ZOMBIE, // moderate HP, moderate speed
        TROLL, // high HP, moves slowly
        BABY_ZOMBIE, // low HP, moves fast
        MONSTER, // high HP, high damage, moves fast
        BOSS, // very high HP, very high damage, moves very slow, only one in the arena
        ENERGY_DRINK, // heals player/mob
        STRENGTH_POTION, // increases damage of player/mob
    };

} // namespace core

#endif // CORE_ENTITY_TYPE_HPP
