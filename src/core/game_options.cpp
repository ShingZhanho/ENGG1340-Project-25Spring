#include <core/game_options.hpp>
#include <core/arena_reader.hpp>
#include <util/log.hpp>

#include <fstream>

namespace core {

//  -- built-in GameOptions structs -----------------------------------

    namespace DefaultGameOptions {

        GameOptions EASY() {
            GameOptions options({
                100,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE, EntityType::BABY_ZOMBIE}, //  MobTypesGenerated
                10,         // MaxMobs
                250,        // MobSpawnInterval
                0,          // DifficultyLevel
            });
            util::WriteToLog("Trying to load built-in arena for level EASY...", "DefaultGameOptions::EASY()");
            auto fs = std::ifstream("res/default_maps/easy.shoot");
            auto reader = ArenaReader(fs);
            options.GameArena = reader.IsSuccess() ? reader.GetArena() : nullptr;
            return options;
        }

        GameOptions MEDIUM() {
            GameOptions options({
                50,         //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE, EntityType::TROLL, EntityType::BABY_ZOMBIE, EntityType::MONSTER}, //  MobTypesGenerated
                15,         // MaxMobs
                150,        // MobSpawnInterval
                1,          // DifficultyLevel
            });
            util::WriteToLog("Trying to load built-in arena for level MEDIUM...", "DefaultGameOptions::EASY()");
            auto fs = std::ifstream("res/default_maps/medium.shoot");
            auto reader = ArenaReader(fs);
            options.GameArena = reader.IsSuccess() ? reader.GetArena() : nullptr;
            return options;
        }

        GameOptions HARD() {
            GameOptions options({
                25,         //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE, EntityType::TROLL, EntityType::BABY_ZOMBIE, EntityType::MONSTER, EntityType::BOSS}, //  MobTypesGenerated
                30,         // MaxMobs
                100,        // MobSpawnInterval
                2,          // DifficultyLevel
            });
            util::WriteToLog("Trying to load built-in arena for level HARD...", "DefaultGameOptions::EASY()");
            auto fs = std::ifstream("res/default_maps/hard.shoot");
            auto reader = ArenaReader(fs);
            options.GameArena = reader.IsSuccess() ? reader.GetArena() : nullptr;
            return options;
        }

    } // namespace GameOptions

} // namespace core