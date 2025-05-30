#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

#include <atomic>
#include <mutex>

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
            //  Terminates the game. Takes one optional argument: reason.
            //  By default, the reason is 0 (game over).
            //  Other acceptable values:
            //      1: player quit
            void Terminate(int reason = 0);
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
            int GetScore();
            //  Returns the reason for termination.
            //  0: game over; 1: player quit;
            int GetTerminateReason() const;
            
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
            //  The exposed PlayerShootEventHandler.
            PlayerShootEventHandler* PlayerShootEventHandlerPtr = nullptr;
            //  The exposed BulletMoveEventHandler.
            BulletMoveEventHandler* BulletMoveEventHandlerPtr = nullptr;
            //  Increments the game clock by 1 tick.
            void IncrementGameClock();
            //  Returns the game clock.
            long long GetGameClock() const;

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
            //  The mutex for the game.
            std::mutex gameMutex;
            //  The clock of the game. Unit: ticks.
            std::atomic<long long> gameClock = 0;
            //  Records the reason for termination.
            int terminateReason = -1;
    };

} // namespace core

#endif // CORE_ENTITY_HPP