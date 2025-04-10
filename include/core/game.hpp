#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

#include <set>

#include <core/arena.hpp>
#include <core/event_handler.hpp>

namespace core {

    //  Forward declarations
    class Arena;
    class EventHandler;
    
    //  The options for the game.
    typedef struct GameOptions {
        //  The initial health of the player.
        int PlayerHp;
        //  The game arena. Built-in GameOptions should set this to nullptr.
        //  Only provide arena if loaded from a user-defined file.
        Arena* GameArena;
        //  The types of mobs that will be spawned in the game.
        //  Although the set uses the EntityType enum, it should only include types that are actually mobs.
        std::set<core::Entity::EntityType> MobTypesGenerated;
    } GameOptions;

    //  The main object representing the whole round.
    class Game {
        public:
            //  Constructor
            Game(GameOptions options);
            //  Destructor
            ~Game();

            //  The entry point of the game. Returns the final score.
            int Run();

            void ChangeScore(int delta);
            int GetScore() const;
            
            //  Initialise the game arena. If the arena is provided in the GameOptions,
            //  that arena is used instead. This function does nothing if the arena is
            //  already initialised.
            void InitialiseArena();
            //  Returns the arena of the game.
            Arena* GetArena() const;
            //  Returns the game options.
            GameOptions GetOptions() const;

        private:
            //  The score. Initial score is 0.
            int score = 0;
            //  The arena of the game.
            Arena* arena;
            bool arenaInitialised = false;
            //  The root event.
            EventHandler* runEventHandler;
            //  The game options.
            GameOptions options;
    };

    //  Built-in GameOptions
    namespace DefaultGameOptions {
        extern const GameOptions EASY;
        extern const GameOptions MEDIUM;
        extern const GameOptions HARD;
    } // namespace DefaultGameOptions

} // namespace core

#endif // CORE_ENTITY_HPP