#ifndef CORE_ENTITY_TYPE_HPP
#define CORE_ENTITY_TYPE_HPP

namespace core {

    enum class EntityType {
        ABSTRACT_ENTITY,// abstract class for all entities
        ABSTRACT_BLOCK,// abstract class for all blocks
        ABSTRACT_MOB,// abstract class for all mobs
        ABSTRACT_COLLECTIBLE, // abstract class for all collectibles
        PLAYER_BULLET,// The bullet shooted by the player
        WALL,// The boundaries of arena
        AIR,// Point in arena with nothing
        PLAYER,// moderate HP, moderate speed
        ZOMBIE, // moderate HP, moderate speed
        TROLL, // high HP, moves slowly
        BABY_ZOMBIE, // low HP, moves fast
        MONSTER, // high HP, high damage, moves fast
        BOSS, // very high HP, very high damage, moves very slow, only one in the arena
        ENERGY_DRINK, // heals player/mob
        STRENGTH_POTION, // increases damage of player/mob
        SHIELD, // temporary protection for player/mob
    };

} // namespace core

#endif // CORE_ENTITY_TYPE_HPP
