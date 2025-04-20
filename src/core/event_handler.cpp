//  core components
#include <core/game.hpp>
#include <core/event_handler.hpp>
#include <core/entity.hpp>
#include <core/arena.hpp>
#include <core/point.hpp>

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
#include <unordered_map>
#include <queue>
#include <string>

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
            case Direction::UP_LEFT:
                player->Move({player->GetPosition().x - 1, player->GetPosition().y - 1});
                break;
            case Direction::UP_RIGHT:
                player->Move({player->GetPosition().x + 1, player->GetPosition().y - 1});
                break;
            case Direction::DOWN_LEFT:
                player->Move({player->GetPosition().x - 1, player->GetPosition().y + 1});
                break;
            case Direction::DOWN_RIGHT:
                player->Move({player->GetPosition().x + 1, player->GetPosition().y + 1});
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
            new MobMoveEventHandler(game)
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
        GetGame()->IncrementGameClock();
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
        auto entities = GetGame()->GetArena()->GetMappedEntities();
        
        // Iterate through the entity index to count mobs
        for (auto it = entities.begin(); it != entities.end(); ++it) {
            if (Entity::IsType(*it, EntityType::ABSTRACT_MOB)) {
                count++;
            }
        }
        return count;
    }
    
    void MobGenerateEventHandler::spawnMob() {
        Arena* arena = GetGame()->GetArena();
        
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
            auto it = GetGame()->GetOptions()->MobTypesGenerated.begin();
            std::advance(it, std::rand() % GetGame()->GetOptions()->MobTypesGenerated.size());
            EntityType mobType = *it;
            AbstractMob* mob;
            switch (mobType) {
                case EntityType::ZOMBIE:
                    mob = new Zombie(spawnPos, arena);
                    break;
                case EntityType::TROLL:
                    mob = new Troll(spawnPos, arena);
                    break;
                default:
                    util::WriteToLog("Unknown mob type: " + std::to_string(static_cast<int>(mobType)), "MobGenerateEventHandler::spawnMob()");
                    return;
            }
            bool success = arena->SetPixelWithIdSafe(spawnPos, mob);
            if (success) {
                util::WriteToLog("Mob spawned successfully at (" + std::to_string(spawnPos.x) + ", " + std::to_string(spawnPos.y) + ")", "MobGenerateEventHandler::spawnMob()");
            } else {
                util::WriteToLog("Failed to spawn mob at (" + std::to_string(spawnPos.x) + ", " + std::to_string(spawnPos.y) + ")", "MobGenerateEventHandler::spawnMob()");
                delete mob;
            } 
            break;
        }
    }

    //  END: MobGenerateEventHandler
    
    //  BEGIN: MobMoveEventHandler
    
    MobMoveEventHandler::MobMoveEventHandler(Game* game) : EventHandler(game) {
        playerPrevPos = {-1, -1}; // Initial value to ensure pathfinding is done at the start
        prevMobCount = 0;
    }

    void MobMoveEventHandler::Fire() {
        util::WriteToLog("MobMoveEvent triggered", "MobMoveEventHandler::Fire()");
        execute();
        EventHandler::Fire();
    }
    
    void MobMoveEventHandler::execute() {
        auto playerPos = GetGame()->GetArena()->GetPixelById(0)->GetPosition();
        // Move all mobs
        auto entities = GetGame()->GetArena()->GetEntitiesOfType(EntityType::ABSTRACT_MOB);
        int mobCount = 0;
        for (auto entity : entities) {
            if (!Entity::IsType(entity, EntityType::ABSTRACT_MOB)) continue;
            auto mob = dynamic_cast<AbstractMob*>(entity);
            if (mob == nullptr) continue;
            mobCount++;
            mob->Move();
        }
        if (playerPos == playerPrevPos && mobCount == prevMobCount) return; // No need to recalculate path if player hasn't moved
        playerPrevPos = playerPos;
        prevMobCount = mobCount;

        // Perform pathfinding for all mobs
        for (auto entity : entities) {
            if (!Entity::IsType(entity, EntityType::ABSTRACT_MOB)) continue;
            auto mob = dynamic_cast<AbstractMob*>(entity);
            if (mob == nullptr) continue;
            mob->Path = findPath(GetGame()->GetArena(), mob->GetPosition(), playerPos);
        }
    }

    std::list<Point> MobMoveEventHandler::findPath(Arena* arena, Point start, Point end) {
        // References:
        // - https://www.redblobgames.com/pathfinding/a-star/introduction.html
        // - https://www.redblobgames.com/pathfinding/a-star/implementation.html#cpp-astar
        typedef std::pair<int, Point> Node;
        std::unordered_map<Point, Point> cameFrom;
        std::unordered_map<Point, int> costSoFar;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;

        frontier.emplace(0, start);
        cameFrom[start] = start;
        costSoFar[start] = 0;

        while (!frontier.empty()) {
            Point current = frontier.top().second;
            frontier.pop();
            if (current == end) break;

            for (auto next : getNeighbours(current)) {
                if (Entity::IsType(arena->GetPixel(next), EntityType::WALL)) continue; // Skip walls
                int newCost = costSoFar[current] + 1;
                if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    frontier.emplace(newCost + heuristic(next, end), next);
                    cameFrom[next] = current;
                }
            }
        }

        // Reconstruct path
        std::list<Point> path;
        Point current = end; // include end point (player position) in path for collision
        if (cameFrom.find(end) == cameFrom.end()) return path; // No path found
        while (current != start) {
            path.push_front(current);
            current = cameFrom[current];
        }

        return path;
    }

    int MobMoveEventHandler::heuristic(Point a, Point b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    std::list<Point> MobMoveEventHandler::getNeighbours(Point point) {
        std::list<Point> neighbours;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = point.x + dx;
                int ny = point.y + dy;
                if (nx >= 1 && nx < ARENA_WIDTH - 1 && ny >= 1 && ny < ARENA_HEIGHT - 1) {
                    neighbours.push_back({nx, ny});
                }
            }
        }
        return neighbours;
    }
    
    //  END: MobMoveEventHandler
}
