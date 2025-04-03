#ifndef CORE_EVENTHANDLER_HPP
#define CORE_EVENTHANDLER_HPP

#include <vector>

namespace core {

    //  Forward declarations
    class Game; // TODO: Remove this line when Game is separated in its own file.

    //  The abstract EventHandler.
    //  Eventhandlers are where your actual code lives. A EventHandler can be fired to exeucte the event.
    //  Events can have subevents. When event is fired, all its recursive subevents are fired in a DFS pattern.
    class EventHandler {
        public:
            //  Constructor and destructor.
            EventHandler(Game* game);
            virtual ~EventHandler();

            Game* GetGame();

            //  Triggers the event.
            virtual void Fire() final;

        protected:
            //  The function that is called when the event is fired. It should be overriden.
            virtual void Execute() = 0;

        private:
            Game* game;
            //  The subevents.
            std::vector<EventHandler*> subevents;
    };
}

#endif // CORE_EVENTHANDLER_HPP