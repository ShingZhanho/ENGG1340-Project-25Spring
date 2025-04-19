#include <core/game_options.hpp>

namespace core {

//  -- built-in GameOptions structs -----------------------------------

    namespace DefaultGameOptions {

        GameOptions EASY() {
            return GameOptions({
                100,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE}, //  MobTypesGenerated
                10          // MaxMobs
            });
        }

        GameOptions MEDIUM() {
            return GameOptions({
                50,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE}, //  MobTypesGenerated
                15          // MaxMobs
            });
        }

        GameOptions HARD() {
            return GameOptions({
                25,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE}, //  MobTypesGenerated
                30          // MaxMobs
            });
        }

    } // namespace GameOptions

} // namespace core