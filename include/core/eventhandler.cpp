//  ASCII-only is recommended for compatibility.
//  All comments begin with // then 2 spaces. In total that's 4 character, the length of a typical tab.
//  YOU MUST COMMENT YOUR CODE.

#ifndef CORE_EVENTHANDLER_HPP
#define CORE_EVENTHANDLER_HPP

#include <vector>
#include <chrono>
#include <thread>

namespace core {
    //  Forward declarations
    class Game;
    class Arena;
    class Entity;
    class Player;

    //  The abstract EventHandler.
    //  Eventhandlers are where your actual code lives. A EventHandler can be fired to execute the event.
    //  Events can have subevents. When event is fired, all its recursive subevents are fired in a DFS pattern.
    class EventHandler {
        public:
        //  A link to the game.
        Game* game;
        
        //  Base constructor.
        EventHandler(Game* game);
        
        //  subEvents: Some event might trigger other events after its execution.
        //      This allows event tree triggering.
        //      Subevents are deleted cascadely. You just need to new them in the constructor.
        std::vector<EventHandler*> subevents;
        
        //  The event code.
        virtual void execute();
        
        //  The fire method fires the event. It should be the only method called outside, also not overriden.
        void fire();
        
        //  Get the game pointer
        Game* GetGame();
        
        //  The virtual destructor helps you automatically destruct all subevents.
        //  It's why they're called subevents and other than this exception you're responsible for deleting everything you new.
        virtual ~EventHandler();
        
        //  Renamed fire to Fire to match the existing code style
        void Fire();
    };

    //  The event fired when calling game.run(). The root event.
    class RunEventHandler: public EventHandler {
        public:
        InitializeEventHandler* initializeEventHandler;
        TickEventHandler* tickEventHandler;
        
        RunEventHandler(Game* game);
        void execute() override;
    };

    //  The game initializing EventHandler. Called after Game() constructor, before TickEventHandler.
    class InitializeEventHandler: public EventHandler {
        public:
        InitializeEventHandler(Game* game);
        void execute() override;
    };

    //  The tick event. Called every tick.
    //  Add your event as subevent. The root event itself shouldn't contain any execution.
    class TickEventHandler: public EventHandler {
        public:
        TickEventHandler(Game* game);
        void execute() override;
    };
    
    //  Player movement event handler
    class PlayerMoveEventHandler: public EventHandler {
        public:
        //  Direction enum for player movement
        enum class Direction { UP, DOWN, LEFT, RIGHT };
        
        PlayerMoveEventHandler(Game* game);
        void execute() override;
        
        //  Execute with direction parameter
        void execute(Direction direction);
    };
    
    //  Player shooting event handler
    class PlayerShootEventHandler: public EventHandler {
        public:
        PlayerShootEventHandler(Game* game);
        void execute() override;
    };
    
    //  Mob generation event handler
    class MobGenerateEventHandler: public EventHandler {
        public:
        MobGenerateEventHandler(Game* game);
        void execute() override;
    };
    
    //  Entity movement event handler
    class EntityMoveEventHandler: public EventHandler {
        public:
        EntityMoveEventHandler(Game* game);
        void execute() override;
    };
    
    //  Screen refresh event handler
    class ScreenRefreshEventHandler: public EventHandler {
        public:
        ScreenRefreshEventHandler(Game* game);
        void execute() override;
    };

}  // namespace core

#endif  // CORE_EVENTHANDLER_HPP
