#include <core/game_options.hpp>

namespace core {

//  -- built-in GameOptions structs -----------------------------------

    namespace DefaultGameOptions {

        const GameOptions EASY = GameOptions({
            100,        //  PlayerHp
            nullptr,    //  GameArena
            {EntityType::ZOMBIE} //  MobTypesGenerated
        });

        const GameOptions MEDIUM = GameOptions({
            50,        //  PlayerHp
            nullptr,   //  GameArena
            {EntityType::ZOMBIE} //  MobTypesGenerated
        });

        const GameOptions HARD = GameOptions({
            25,        //  PlayerHp
            nullptr,   //  GameArena
            {EntityType::ZOMBIE} //  MobTypesGenerated
        });

    } // namespace GameOptions

} // namespace core