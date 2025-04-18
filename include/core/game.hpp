#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

#include <atomic>
#include <set>

#include <core/arena.hpp>
#include <core/entity.hpp>
#include <core/event_handler.hpp>
#include <core/game_options.hpp>

namespace core {

    //  Forward declarations
    class Arena;
    class EventHandler;
    struct GameOptions;

    //  The main object representing the whole round.
    class Game {
        public:
            //  Constructor
            Game(GameOptions* options);
            //  Destructor
            ~Game();

            //  The entry point of the game. Returns the final score.
            int Run();
            //  Terminates the game.
            void Terminate();
            //  Returns true if the game is running.
            bool IsRunning() const;
            //  Returns true if the game is initialised.
            bool IsInitialised() const;
            //  Signals the game that initialisation is complete.
            void SetInitialisationComplete();
            //  Returns true if the game is fully terminated.
            bool IsTerminated() const;
            //  Signals that the game is fully terminated.
            void SetTerminated();

            void ChangeScore(int delta);
            int GetScore() const;
            
            //  Initialise the game arena. If the arena is provided in the GameOptions,
            //  that arena is used instead. This function does nothing if the arena is
            //  already initialised.
            void InitialiseArena();
            //  Returns the arena of the game.
            Arena* GetArena() const;
            //  Returns the game options.
            GameOptions* GetOptions() const;
            //  The exposed PlayerMovementEventHandler.
            PlayerMoveEventHandler* PlayerMoveEventHandlerPtr = nullptr;

        private:
            //  The score. Initial score is 0.
            int score = 0;
            //  The arena of the game.
            Arena* arena;
            //  The flag indicating whether the arena is initialised.
            bool arenaInitialised = false;
            //  The flag indicating whether the arena is created using new in this class.
            bool arenaIsDynamicallyCreated = false;
            //  The root event.
            EventHandler* runEventHandler;
            //  The game options.
            GameOptions* options;
            //  The flag indicating whether the game is running.
            std::atomic<bool> running = false;
            //  The flag indicating whether the initialisation is complete.
            std::atomic<bool> initialisationComplete = false;
            //  The flag indicating whether the game is fully terminated.
            std::atomic<bool> terminated = false;
    };

} // namespace core

#endif // CORE_ENTITY_HPP