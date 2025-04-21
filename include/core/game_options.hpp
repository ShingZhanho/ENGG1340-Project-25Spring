#ifndef CORE_GAME_OPTIONS_HPP
#define CORE_GAME_OPTIONS_HPP

#include <set>

#include <core/entity_type.hpp>

namespace core {

    //  Forward declarations
    class Arena;
    
    //  The options for the game.
    struct GameOptions {
        //  The initial health of the player.
        int PlayerHp;
        //  The game arena. Built-in GameOptions should set this to nullptr.
        //  Only provide arena if loaded from a user-defined file.
        Arena* GameArena;
        //  The types of mobs that will be spawned in the game.
        //  Although the set uses the EntityType enum, it should only include types that are actually mobs.
        std::set<core::EntityType> MobTypesGenerated;
        //  The maximum number of mobs at any given moment.
        int MaxMobs;
        //  The tick interval between each mob generation.
        //  Measured in ticks (50 ticks per second).
        int MobSpawnInterval;
    };

    //  Built-in GameOptions
    namespace DefaultGameOptions {
        GameOptions EASY();
        GameOptions MEDIUM();
        GameOptions HARD();
    } // namespace DefaultGameOptions

} // namespace core

#endif // CORE_GAME_OPTIONS_HPP