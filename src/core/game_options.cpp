#include <core/game_options.hpp>

namespace core {

//  -- built-in GameOptions structs -----------------------------------

    namespace DefaultGameOptions {

        GameOptions EASY() {
            return GameOptions({
                100,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE} //  MobTypesGenerated}
            });
        }

        GameOptions MEDIUM() {
            return GameOptions({
                50,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE} //  MobTypesGenerated}
            });
        }

        GameOptions HARD() {
            return GameOptions({
                25,        //  PlayerHp
                nullptr,    //  GameArena
                {EntityType::ZOMBIE} //  MobTypesGenerated}
            });
        }

    } // namespace GameOptions

} // namespace core