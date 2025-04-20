#ifndef CORE_EVENTHANDLER_HPP
#define CORE_EVENTHANDLER_HPP

#include <core/arena.hpp>
#include <core/game.hpp>
#include <core/point.hpp>
#include <vector>
#include <thread>

namespace core {

    //  Forward declarations
    class Game;
    class Arena;
    class EventHandler;
    class RunEventHandler;
    class InitialiseEventHandler;
    class TickEventHandler;
    class PlayerMoveEventHandler;
    class PlayerShootEventHandler;
    class MobGenerateEventHandler;
    class MobMoveEventHandler;

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
            enum class Direction { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, STILL };
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

            long long lastSpawnTick;
    };
    
    //  Handles the movement of mobs.
    class MobMoveEventHandler : public EventHandler {
        public:
            //  Constructor
            MobMoveEventHandler(Game* game);
            void Fire() override;

        private:
            //  Executed when the event is fired.
            void execute();
            //  The player's previous position. Used to check if the player has moved
            //  and if pathfinding is needed. Initial value is (-1, -1) to ensure
            //  pathfinding must be done at the start.
            Point playerPrevPos;
            int prevMobCount;
            //  Finds the shortest path from start to end on the given arena using A* algorithm.
            //  Returns a list of points representing the path, where the first point is
            //  the one closest to the start and the last point is the one closest to the end.
            //  The path does not include the start and end points.
            //  Returns an empty list if no path is found.
            static std::list<Point> findPath(Arena* arena, Point start, Point end);
            //  Returns the manhattan distance between two points.
            inline static int heuristic(Point a, Point b);
            //  Returns neighbours iff they are within x:[1, ARENA_WIDTH-2] and y:[1, ARENA_HEIGHT-2]
            //  i.e. not on the border. Includes diagonal neighbours.
            inline static std::list<Point> getNeighbours(Point point);
    };
}

#endif // CORE_EVENTHANDLER_HPP
