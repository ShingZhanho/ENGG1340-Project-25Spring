//  core components
#include <core/game.hpp>
#include <core/event_handler.hpp>
#include <core/entity.hpp>
#include <core/arena.hpp>

// ftxui
#include <ftxui/component/component.hpp>

// ui components
#include <ui/common.hpp>
#include <ui/render_option.hpp>

// standard library
#include <random>
#include <chrono>

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

        //  Create player if it doesn't exist
        //  NOTE: The player MUST be the first non-block entity to have the ID 0.
        if (game->GetArena()->GetPixelById(0) == nullptr){
            game->GetArena()->Replace({15, 50}, new Player({15, 50}, game->GetArena()));
        }
    }

    void InitialiseEventHandler::execute() {}

    //  END: InitialiseEventHandler

    //  BEGIN: PlayerMoveEventHandler

    PlayerMoveEventHandler::PlayerMoveEventHandler(Game* game) : EventHandler(game) {}

    void PlayerMoveEventHandler::Fire() {
        // your code here ...
        //  TODO: implement this function BEFORE this line, remove this comment when you're done
        EventHandler::Fire();
    }

    void PlayerMoveEventHandler::SetDirection(Direction direction) {
        //  TODO: impolement this function AFTER this line, remove this comment when you're done
        // your code here ...
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
    
    MobGenerateEventHandler::MobGenerateEventHandler(Game* game) :
        EventHandler(game),
        lastSpawnTime(std::chrono::steady_clock::now()),
        maxMobs(10) {
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
    }
    
    void EntityMoveEventHandler::execute() {
        // Implement entity movement logic
    }
    
    //  END: EntityMoveEventHandler
    
    //  BEGIN: ScreenRefreshEventHandler
    
    ScreenRefreshEventHandler::ScreenRefreshEventHandler(Game* game) : EventHandler(game) {
    }
    
    void ScreenRefreshEventHandler::execute() {
        auto container = ftxui::Container::Vertical({});
        ui::RenderOption* entityRenderer = nullptr;
        for (int y = 0; y < ARENA_HEIGHT; ++y) {
            auto row = ftxui::Container::Horizontal({});
            for (int x = 0; x < ARENA_WIDTH; ++x) {
                entityRenderer = GetGame()->GetArena()->GetPixel({x, y})->GetRenderOption();
                row->Add(ftxui::Renderer([&] {
                    return entityRenderer->Render();
                }));
            }
            container->Add(row);
        }
        auto ui = ftxui::Renderer(container, [&] {
            return ftxui::vbox({
                ftxui::text("Game Screen") | ftxui::center | ftxui::bold,
                ftxui::separator(),
                container->Render()
            });
        });
        ui::appScreen.Loop(ui);
    }
    
    //  END: ScreenRefreshEventHandler
}
