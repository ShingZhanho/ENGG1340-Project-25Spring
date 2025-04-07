#ifndef CORE_EVENTHANDLER_HPP
#define CORE_EVENTHANDLER_HPP

#include <vector>
#include <chrono>
#include <thread>

namespace core {

    //  Forward declarations
    class Game; // TODO: Remove this line when Game is separated in its own file.
    class EventHandler;
    class RunEventHandler;
    class InitialiseEventHandler;
    class TickEventHandler;
    class PlayerMoveEventHandler;
    class PlayerShootEventHandler;
    class MobGenerateEventHandler;
    class EntityMoveEventHandler;
    class ScreenRefreshEventHandler;

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
            virtual void Fire();

        private:
            Game* game;
            //  The subevents.
            std::vector<EventHandler*> subevents;
            //  Executed when the event is fired.
            void execute();
    };

    //  The event triggered when Game.Run() is called.
    class RunEventHandler : public EventHandler {
        public:
            //  Constructor.
            RunEventHandler(Game* game);

        private:
            InitialiseEventHandler* initialiseEventHandler;
            TickEventHandler* tickEventHandler;
            //  Executed when the event is fired.
            void execute();
    };

    class InitialiseEventHandler : public EventHandler {
        public:
            //  Constructor
            InitialiseEventHandler(Game* game);

        private:
            //  Executed when the event is fired.
            void execute();
    };

    class PlayerMoveEventHandler : public EventHandler {
        public:
            PlayerMoveEventHandler(Game* game);
            enum class Direction;

        private:
            //  Executed when the event is fired.
            void execute(Direction direction);
    };
}

#endif // CORE_EVENTHANDLER_HPP
