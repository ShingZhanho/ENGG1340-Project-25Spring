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

        protected:
            //  The subevents.
            std::vector<EventHandler*> subevents;

        private:
            Game* game;
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
            enum class Direction { UP, DOWN, LEFT, RIGHT };

        private:
            //  Executed when the event is fired.
            void execute(Direction direction);
    };

    //  The tick event. Called every tick.
    //  Add your event as subevent. The root event itself shouldn't contain any execution.
    class TickEventHandler : public EventHandler {
        public:
            //  Constructor
            TickEventHandler(Game* game);

        private:
            //  Executed when the event is fired.
            void execute();
    };
    
    //  Player shooting event handler
    class PlayerShootEventHandler : public EventHandler {
        public:
            //  Constructor
            PlayerShootEventHandler(Game* game);

        private:
            //  Executed when the event is fired.
            void execute();
    };
    
    //  Mob generation event handler
    class MobGenerateEventHandler : public EventHandler {
        public:
            //  Constructor
            MobGenerateEventHandler(Game* game);

        private:
            //  Executed when the event is fired.
            void execute();
    };
    
    //  Entity movement event handler
    class EntityMoveEventHandler : public EventHandler {
        public:
            //  Constructor
            EntityMoveEventHandler(Game* game);

        private:
            //  Executed when the event is fired.
            void execute();
    };
    
    //  Screen refresh event handler
    class ScreenRefreshEventHandler : public EventHandler {
        public:
            //  Constructor
            ScreenRefreshEventHandler(Game* game);

        private:
            //  Executed when the event is fired.
            void execute();
    };
}

#endif // CORE_EVENTHANDLER_HPP
