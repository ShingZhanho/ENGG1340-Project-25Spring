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
#include <cstdlib>
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
                util::WriteToLog("Waiting for game initialisation...", "RunEventHandler::execute() {thread: tickThread}");
            }
            //  The game will end with throw endType so there is no need of condition testing.
            util::WriteToLog("Game initialised. Starting tick event handler loop...", "RunEventHandler::execute() {thread: tickThread}");
            while (GetGame()->IsRunning()) {
                using namespace std::chrono_literals;
                tickEventHandler->Fire();
                std::this_thread::sleep_for(1s);
            }
            util::WriteToLog("Game loop exited. Terminating tickThread...", "RunEventHandler::execute() {thread: tickThread}");
            GetGame()->SetTerminated();
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

        //  Pollinate the random number generator seed
        util::WriteToLog("Seeding random number generator...", "InitialiseEventHandler::InitialiseEventHandler()");
        std::srand(std::chrono::system_clock::now().time_since_epoch().count());

        //  Create player if it doesn't exist
        //  NOTE: The player MUST be the first non-block entity to have the ID 0.
        util::WriteToLog("Checking player ID...", "InitialiseEventHandler::InitialiseEventHandler()");
        if (game->GetArena()->GetPixelById(0) == nullptr) {
            util::WriteToLog("Entity with ID 0 not found. Creating player...", "InitialiseEventHandler::InitialiseEventHandler()");
            auto player = new Player({ARENA_WIDTH / 2, ARENA_HEIGHT / 2}, game->GetArena(), game->GetOptions()->PlayerHp);
            game->GetArena()->SetPixelWithId(player->GetPosition(), player);
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
            new EntityMoveEventHandler(game)
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
        ui::appScreen.Post(ftxui::Event::Custom);
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
        lastSpawnTime(std::chrono::steady_clock::now()) {
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
        if (elapsedTime < 3) return;

        if (countMobs() >= GetGame()->GetOptions()->MaxMobs) return;

        util::WriteToLog("Spawning mob...", "MobGenerateEventHandler::execute()");
        spawnMob();
        lastSpawnTime = currentTime;
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
        
        
        // Find a valid spawn position (must be air)
        Point spawnPos = {0, 0};
        int attempts = 0;
        while (attempts < 20) {
            spawnPos.x = std::rand() % ARENA_WIDTH;
            spawnPos.y = std::rand() % ARENA_HEIGHT;
            
            Entity* currentEntity = arena->GetPixel(spawnPos);
            if (!Entity::IsType(currentEntity, EntityType::AIR)) {
                attempts++;
                continue;
            }

            bool success = arena->SetPixelSafe(spawnPos, new Zombie(spawnPos, arena)); // TODO: use a random mob type
            if (success)
                util::WriteToLog("Mob spawned successfully at (" + std::to_string(spawnPos.x) + ", " + std::to_string(spawnPos.y) + ")", "MobGenerateEventHandler::spawnMob()");
            else
                util::WriteToLog("Failed to spawn mob at (" + std::to_string(spawnPos.x) + ", " + std::to_string(spawnPos.y) + ")", "MobGenerateEventHandler::spawnMob()");
            break;
        }
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
}
