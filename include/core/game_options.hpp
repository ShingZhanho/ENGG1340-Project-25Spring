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
    };

    //  Built-in GameOptions
    namespace DefaultGameOptions {
        extern const GameOptions EASY;
        extern const GameOptions MEDIUM;
        extern const GameOptions HARD;
    } // namespace DefaultGameOptions

} // namespace core

#endif // CORE_GAME_OPTIONS_HPP