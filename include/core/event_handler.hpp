#ifndef CORE_EVENTHANDLER_HPP
#define CORE_EVENTHANDLER_HPP

#include <core/arena.hpp>
#include <core/game.hpp>
#include <vector>
#include <chrono>
#include <thread>

namespace core {

    //  Forward declarations
    class Game;
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
            //  Destructor.
            ~RunEventHandler();
            //  Triggers the event.
            void Fire() override;

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
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute();
    };

    class PlayerMoveEventHandler : public EventHandler {
        public:
            PlayerMoveEventHandler(Game* game);
            enum class Direction { UP, DOWN, LEFT, RIGHT, STILL };
            //  Sets the direction of the player. Call this method before calling Fire().
            //  The Fire() method will then use this direction to move the player.
            void SetDirection(Direction direction);
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute(Direction direction);
            //  Indicates the direction of the player.
            Direction movementDirection = Direction::STILL;
    };

    //  The tick event. Called every tick.
    //  Add your event as subevent. The root event itself shouldn't contain any execution.
    class TickEventHandler : public EventHandler {
        public:
            //  Constructor
            TickEventHandler(Game* game);
            //  Destructor
            ~TickEventHandler();
            //  Triggers the event.
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute();
            //  The internal PlayerMoveEventHandler. This event will be exposed to UI and NOT
            //  triggered by the tick event.
            PlayerMoveEventHandler* playerMoveEventHandler;
    };
    
    //  Player shooting event handler
    class PlayerShootEventHandler : public EventHandler {
        public:
            //  Constructor
            PlayerShootEventHandler(Game* game);
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute();
    };
    
    //  Mob generation event handler
    class MobGenerateEventHandler : public EventHandler {
        public:
            //  Constructor
            MobGenerateEventHandler(Game* game);
            void Fire() override;
            
        private:
            //  Executed when the event is fired.
            void execute();
            int countMobs();
            void spawnMob();
            const int maxMobs;
    
            // Random number generator
            // std::mt19937 rng;
            // TODO: FIX: random number should be generated using the method taught in the course
            // i.e. via rand() in <cstdlib> and srand() to seed the random number generator

            std::chrono::steady_clock::time_point lastSpawnTime;
    };
    
    //  Entity movement event handler
    class EntityMoveEventHandler : public EventHandler {
        public:
            //  Constructor
            EntityMoveEventHandler(Game* game);
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute();
    };
    
    //  Screen refresh event handler
    class ScreenRefreshEventHandler : public EventHandler {
        public:
            //  Constructor
            ScreenRefreshEventHandler(Game* game);
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute();
    };
}

#endif // CORE_EVENTHANDLER_HPP
