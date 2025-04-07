#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

#include <core/arena.hpp>
#include <core/event_handler.hpp>

namespace core {

    //  Forward declarations
    class Arena;
    class EventHandler;

    //  The main object representing the whole round.
    class Game {
        public:
            //  Constructor
            Game();
            //  Destructor
            ~Game();

            //  The entry point of the game. Returns the final score.
            int Run();

            void ChangeScore(int delta);
            int GetScore() const;
            
            void InitialiseArena();
            Arena* GetArena() const;

        private:
            //  The score. Initial score is 0.
            int score = 0;
            //  The arena of the game.
            Arena* arena;
            bool arenaInitialised = false;
            //  The root event.
            EventHandler* runEventHandler;
    };

}

#endif // CORE_ENTITY_HPP