//  core components
#include <core/game.hpp>
#include <core/event_handler.hpp>
#include <core/entity.hpp>
#include <core/arena.hpp>

// ftxui
#include <ftxui/component/component.hpp>

// ui components
#include <ui/common.hpp>
#include <ui/game_ui_renderer.hpp>
#include <ui/render_option.hpp>

// util components
#include <util/log.hpp>

// standard library
#include <random>
#include <chrono>
#include <thread>

namespace core {

    //  BEGIN: EventHandler

    EventHandler::EventHandler(Game* game) : game(game) { }

    EventHandler::~EventHandler() {
        util::WriteToLog("Deleting subevents...", "EventHandler::~EventHandler()");
        for (auto subevent : subevents) delete subevent;
    }

    Game* EventHandler::GetGame() {
        return game;
    }

    void EventHandler::Fire() {
        execute();
        for (auto it = subevents.begin(); it != subevents.end(); ++it) {
            (*it)->Fire();
        }
    }

    void EventHandler::execute() { }

    //  END: EventHandler

    //  BEGIN: RunEventHandler

    RunEventHandler::RunEventHandler(Game* game) : EventHandler(game) {
        util::WriteToLog("Constructing child InitialiseEventHandler and TickEventHandler", "RunEventHandler::RunEventHandler()");
        initialiseEventHandler = new InitialiseEventHandler(game);
        tickEventHandler = new TickEventHandler(game);
    }

    RunEventHandler::~RunEventHandler() {
        util::WriteToLog("Deleting child InitialiseEventHandler and TickEventHandler", "RunEventHandler::~RunEventHandler()");
        delete initialiseEventHandler;
        delete tickEventHandler;
    }

    void RunEventHandler::Fire() {
        util::WriteToLog("RunEvent triggered", "RunEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }

    void RunEventHandler::execute() {
        //  Initialize the game.
        initialiseEventHandler->Fire();
        util::WriteToLog("InitialiseEventHandler fire completed.", "RunEventHandler::execute()");

        //  Run the tick event handler in a separate thread.
        std::thread tickThread([&] {
            util::WriteToLog("Starting tick event handler thread...", "RunEventHandler::execute() {thread: tickThread}");
            while (GetGame()->IsInitialised() == false) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(500ms); // wait for the game to be initialised
            }
            //  The game will end with throw endType so there is no need of condition testing.
            while (GetGame()->IsRunning()) {
                using namespace std::chrono_literals;
                tickEventHandler->Fire();
                std::this_thread::sleep_for(1s);
            }
        });

        //  Run the game UI renderer.
        ui::publicGameUIRenderer->StartRenderLoop();
        util::WriteToLog("Game UI renderer started. Detaching from tickThread...", "RunEventHandler::execute()");
        tickThread.detach();
    }

    //  END: RunEventHandler

    //  BEGIN: InitialiseEventHandler

    InitialiseEventHandler::InitialiseEventHandler(Game* game) : EventHandler(game) {
        util::WriteToLog("Initialising Game Arena...", "InitialiseEventHandler::InitialiseEventHandler()");
        game->InitialiseArena();
        //  Setup arena layout (walls etc.)
        //  TODO: set layout according to level difficulty

        //  Create player if it doesn't exist
        //  NOTE: The player MUST be the first non-block entity to have the ID 0.
        util::WriteToLog("Checking player ID...", "InitialiseEventHandler::InitialiseEventHandler()");
        if (game->GetArena()->GetPixelById(0) == nullptr) {
            util::WriteToLog("Entity with ID 0 not found. Creating player...", "InitialiseEventHandler::InitialiseEventHandler()");
            game->GetArena()->SetPixelWithId({15, 50}, new Player({15, 50}, game->GetArena(), game->GetOptions()->PlayerHp));
        } else {
            // set player HP by reconstructing the player
            auto playerPoint = game->GetArena()->GetPixelById(0)->GetPosition();
            game->GetArena()->ReplaceWithId(0, new Player(playerPoint, game->GetArena(), game->GetOptions()->PlayerHp));
        }
        util::WriteToLog("InitialiseEventHandler constructed.", "InitialiseEventHandler::InitialiseEventHandler()");

        game->SetInitialisationComplete();
    }

    void InitialiseEventHandler::Fire() {
        util::WriteToLog("InitialiseEvent triggered", "InitialiseEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }

    void InitialiseEventHandler::execute() {}

    //  END: InitialiseEventHandler

    //  BEGIN: PlayerMoveEventHandler

    PlayerMoveEventHandler::PlayerMoveEventHandler(Game* game) : EventHandler(game) {
        util::WriteToLog("Constructing PlayerMoveEventHandler", "PlayerMoveEventHandler::PlayerMoveEventHandler()");
        
    }

    void PlayerMoveEventHandler::Fire() {
        execute(movementDirection);
        EventHandler::Fire();
    }

    void PlayerMoveEventHandler::SetDirection(Direction direction) {
        movementDirection = direction;
    }

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
                SetDirection(Direction::STILL);
                break;
        }

        //  Redraw UI
        ui::appScreen.PostEvent(ftxui::Event::Custom);
    }

    // END: PlayerMoveEventHandler
    
    //  BEGIN: TickEventHandler
    
    TickEventHandler::TickEventHandler(Game* game) : EventHandler(game) {
        //  Add all subevent handlers
        util::WriteToLog("Adding children event handlers", "TickEventHandler::TickEventHandler()");
        subevents = {
            new PlayerShootEventHandler(game),
            new MobGenerateEventHandler(game),
            new EntityMoveEventHandler(game),
            new ScreenRefreshEventHandler(game)
        };
        playerMoveEventHandler = new PlayerMoveEventHandler(game);
        game->PlayerMoveEventHandlerPtr = playerMoveEventHandler; // expose handler to UI
    }

    TickEventHandler::~TickEventHandler() {
        util::WriteToLog("Deleting internal PlayerMoveEventHandler", "TickEventHandler::~TickEventHandler()");
        delete playerMoveEventHandler;
    }

    void TickEventHandler::Fire() {
        util::WriteToLog("TickEvent triggered", "TickEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }
    
    void TickEventHandler::execute() {
        // Execute operations needed for each tick
    }
    
    //  END: TickEventHandler
    
    //  BEGIN: PlayerShootEventHandler
    
    PlayerShootEventHandler::PlayerShootEventHandler(Game* game) : EventHandler(game) {
        // util::WriteToLog("Constructing PlayerShootEventHandler", "PlayerShootEventHandler::PlayerShootEventHandler()");
    }

    void PlayerShootEventHandler::Fire() {
        // util::WriteToLog("PlayerShootEvent triggered", "PlayerShootEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }
    
    void PlayerShootEventHandler::execute() {
        // Implement player shooting logic
    }
    
    //  END: PlayerShootEventHandler
    
    //  BEGIN: MobGenerateEventHandler
    
    MobGenerateEventHandler::MobGenerateEventHandler(Game* game) :
        EventHandler(game),
        lastSpawnTime(std::chrono::steady_clock::now()),
        maxMobs(10) {
        // util::WriteToLog("Constructing MobGenerateEventHandler", "MobGenerateEventHandler::MobGenerateEventHandler()");
    }

    void MobGenerateEventHandler::Fire() {
        // util::WriteToLog("MobGenerateEvent triggered", "MobGenerateEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }
    
    void MobGenerateEventHandler::execute() {
        // Implement mob generation logic
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds> (
            currentTime - lastSpawnTime).count();
        
        // Check if 3 seconds have passed since last spawn
        if (elapsedTime >= 3) {
            // Count current mobs in the arena
            int currentMobCount = countMobs();
            
            // If we have less than max mobs, spawn a new one
            if (currentMobCount < maxMobs) {
                spawnMob();
                lastSpawnTime = currentTime;
            }
        }
    }
    
    int MobGenerateEventHandler::countMobs() {
        int count = 0;
        Arena* arena = GetGame()->GetArena();
        
        // Iterate through the entity index to count mobs
        for (const auto& pair : arena->entityIndex) {
            if (Entity::IsType(pair.second, EntityType::ABSTRACT_MOB)) {
                count++;
            }
        }
        
        return count;
    }
    
    void MobGenerateEventHandler::spawnMob() {
        Arena* arena = GetGame()->GetArena();

        // TODO: Rewrite this function to use the method taught in the course
        
        
        // // Find a valid spawn position (must be air)
        // Point spawnPos;
        // bool validPosition = false;
        
        // // Distribution for random coordinates within valid area
        // std::uniform_int_distribution<int> xDist(minX, maxX);
        // std::uniform_int_distribution<int> yDist(minY, maxY);
        
        // // Try up to 20 times to find a valid position
        // for (int attempts = 0; attempts < 20; attempts++) {
        //     spawnPos.x = xDist(rng);
        //     spawnPos.y = yDist(rng);
            
        //     Entity* currentEntity = arena->GetPixel(spawnPos);
        //     // Check if position is empty (air)
        //     if (dynamic_cast<Air*>(currentEntity) != nullptr) {
        //         validPosition = true;
        //         break;
        //     }
        // }
        
        // // If we found a valid position, spawn the mob
        // if (validPosition) {
        //     Mob* newMob = new Mob(spawnPos, arena);
        //     arena->SetPixelWithId(spawnPos, newMob);
            
        //     // Optional: Log the spawn
        //     // std::cout << "Spawned mob at (" << spawnPos.x << ", " << spawnPos.y << ")" << std::endl;
        // }
    }

    
    
    //  END: MobGenerateEventHandler
    
    //  BEGIN: EntityMoveEventHandler
    
    EntityMoveEventHandler::EntityMoveEventHandler(Game* game) : EventHandler(game) {
        // util::WriteToLog("Constructing EntityMoveEventHandler", "EntityMoveEventHandler::EntityMoveEventHandler()");
    }

    void EntityMoveEventHandler::Fire() {
        // util::WriteToLog("EntityMoveEvent triggered", "EntityMoveEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }
    
    void EntityMoveEventHandler::execute() {
        // Implement entity movement logic
    }
    
    //  END: EntityMoveEventHandler
    
    //  BEGIN: ScreenRefreshEventHandler
    
    ScreenRefreshEventHandler::ScreenRefreshEventHandler(Game* game) : EventHandler(game) {
        // util::WriteToLog("Constructing ScreenRefreshEventHandler", "ScreenRefreshEventHandler::ScreenRefreshEventHandler()");
    }

    void ScreenRefreshEventHandler::Fire() {
        // util::WriteToLog("ScreenRefreshEvent triggered", "ScreenRefreshEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }
    
    void ScreenRefreshEventHandler::execute() {
        // ui::GameUIRenderer renderer(GetGame());
        // renderer.RefreshUI();
    }
    
    //  END: ScreenRefreshEventHandler
}
