//  core components
#include <core/eventhandler.hpp>
#include <core/entity.hpp>

namespace core {

    //  BEGIN: EventHandler

    EventHandler::EventHandler(Game* game) : game(game) { }

    EventHandler::~EventHandler() {
        for (auto subevent : subevents) delete subevent;
    }

    Game* EventHandler::GetGame() {
        return game;
    }

    void EventHandler::Fire() {
        execute();
        for (auto subevent : subevents) subevent->Fire();
    }

    void EventHandler::execute() { }

    //  END: EventHandler

    //  BEGIN: RunEventHandler

    RunEventHandler::RunEventHandler(Game* game) : EventHandler(game) {
        initialiseEventHandler = new InitialiseEventHandler(game);
        tickEventHandler = new TickEventHandler(game);
    }

    void RunEventHandler::execute() {
        //  Initialize the game.
        initialiseEventHandler->Fire();
        //  Run the game.
        //  The game will end with throw endType so there is no need of condition testing.
        while (true) {
            auto now = std::chrono::system_clock::now();
            tickEventHandler->Fire();
            //  Time interval between ticks.
            std::this_thread::sleep_until(now + std::chrono::milliseconds(20));
        }
    }

    //  END: RunEventHandler

    //  BEGIN: InitialiseEventHandler

    InitialiseEventHandler::InitialiseEventHandler(Game* game) : EventHandler(game) {
        game->InitialiseArena();
        //  Setup arena layout (walls etc.)
        //  TODO: set layout according to level difficulty

        //  Create player
        //  NOTE: The player MUST be the first non-block entity to have the ID 0.
        game->GetArena()->Replace({15, 50}, new Player({15, 50}, game->GetArena(), 5));
    }

    void InitialiseEventHandler::execute() {}

    //  END: InitialiseEventHandler

    //  BEGIN: PlayerMoveEventHandler

    PlayerMoveEventHandler::PlayerMoveEventHandler(Game* game) : EventHandler(game) {}

    enum class PlayerMoveEventHandler::Direction { UP, DOWN, LEFT, RIGHT };

    void PlayerMoveEventHandler::execute(Direction direction){
        //  Get player entity
        auto player = dynamic_cast<Player*>(GetGame()->GetArena()->GetPixelById(0));
        if (!player) return;

        //  Move
        switch (direction) {
            case Direction::UP:
                player->Move({player->GetPosition().x, player->GetPosition().y - 1});
                break;
            case Direction::DOWN:
                player->Move({player->GetPosition().x, player->GetPosition().y + 1});
                break;
            case Direction::LEFT:
                player->Move({player->GetPosition().x - 1, player->GetPosition().y});
                break;
            case Direction::RIGHT:
                player->Move({player->GetPosition().x + 1, player->GetPosition().y});
                break;
            default:
                //  Invalid direction
                break;
        }
    }

    // END: PlayerMoveEventHandler
    
    //  BEGIN: TickEventHandler
    
    TickEventHandler::TickEventHandler(Game* game) : EventHandler(game) {
        //  Add all subevent handlers
        subevents = {
            new PlayerMoveEventHandler(game),
            new PlayerShootEventHandler(game),
            new MobGenerateEventHandler(game),
            new EntityMoveEventHandler(game),
            new ScreenRefreshEventHandler(game)
        };
    }
    
    void TickEventHandler::execute() {
        // Execute operations needed for each tick
    }
    
    //  END: TickEventHandler
    
    //  BEGIN: PlayerShootEventHandler
    
    PlayerShootEventHandler::PlayerShootEventHandler(Game* game) : EventHandler(game) {
    }
    
    void PlayerShootEventHandler::execute() {
        // Implement player shooting logic
    }
    
    //  END: PlayerShootEventHandler
    
    //  BEGIN: MobGenerateEventHandler
    
    MobGenerateEventHandler::MobGenerateEventHandler(Game* game) : EventHandler(game) {
    }
    
    void MobGenerateEventHandler::execute() {
        // Implement mob generation logic
    }
    
    //  END: MobGenerateEventHandler
    
    //  BEGIN: EntityMoveEventHandler
    
    EntityMoveEventHandler::EntityMoveEventHandler(Game* game) : EventHandler(game) {
    }
    
    void EntityMoveEventHandler::execute() {
        // Implement entity movement logic
    }
    
    //  END: EntityMoveEventHandler
    
    //  BEGIN: ScreenRefreshEventHandler
    
    ScreenRefreshEventHandler::ScreenRefreshEventHandler(Game* game) : EventHandler(game) {
    }
    
    void ScreenRefreshEventHandler::execute() {
        // Implement screen refresh logic
    }
    
    //  END: ScreenRefreshEventHandler
}
