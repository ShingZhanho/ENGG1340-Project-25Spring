#include <core/game_options.hpp>

namespace core {

//  -- built-in GameOptions structs -----------------------------------

    namespace DefaultGameOptions {

        GameOptions EASY() {
            return GameOptions({
                100,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE}, //  MobTypesGenerated
                10,         // MaxMobs
                250,        // MobSpawnInterval
            });
        }

        GameOptions MEDIUM() {
            return GameOptions({
                50,         //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE, EntityType::TROLL, EntityType::BABY_ZOMBIE}, //  MobTypesGenerated
                15,         // MaxMobs
                150,        // MobSpawnInterval
            });
        }

        GameOptions HARD() {
            return GameOptions({
                25,         //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE, EntityType::TROLL, EntityType::BABY_ZOMBIE}, //  MobTypesGenerated
                30,         // MaxMobs
                100,        // MobSpawnInterval
            });
        }

    } // namespace GameOptions

} // namespace core