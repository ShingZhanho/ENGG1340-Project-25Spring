#include <core/arena.hpp>
#include <core/entity.hpp>

#include <util/log.hpp>

namespace core {

    Arena::Arena() {
        util::WriteToLog("Constructing Arena with default map...", "Arena::Arena()");
        for (int i = 0; i < ARENA_HEIGHT; i++) {
            for (int j = 0; j < ARENA_WIDTH; j++) {
                if (i == 0 || i == ARENA_HEIGHT - 1 || j == 0 || j == ARENA_WIDTH - 1) {
                    // The outermost layer of the arena is always walls
                    pixel[i][j] = new Wall({i, j}, this);
                } else {
                    // Initialize the inner pixels with air
                    pixel[i][j] = new Air({i, j}, this);
                }
            }
        }
        util::WriteToLog("Arena constructed successfully.", "Arena::Arena()");
    }

    Arena::~Arena() {
        util::WriteToLog("Arena destructor called.", "Arena::~Arena()");
        for (int i = 0; i < ARENA_HEIGHT; i++) {
            for (int j = 0; j < ARENA_WIDTH; j++) {
                delete pixel[i][j];
                pixel[i][j] = nullptr;
            }
        }
        util::WriteToLog("Arena destructor completed.", "Arena::~Arena()");
    }

    Entity* Arena::GetPixel(Point p) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        return pixel[p.y][p.x];
    }

    void Arena::SetPixel(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            return;
        }
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = entity;
        entity->SetPosition(p);
    }

    bool Arena::SetPixelSafe(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            return false;
        }
        if (Entity::IsType(pixel[p.y][p.x], EntityType::AIR)) {
            delete pixel[p.y][p.x];
            pixel[p.y][p.x] = entity;
            entity->SetPosition(p);
            return true;
        }
        return false;
    }

    void Arena::SetPixelWithId(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        util::WriteToLog("Attempting to set pixel and assign an ID at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")...", "Arena::SetPixelWithId()");
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            return;
        }
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = entity;
        util::WriteToLog("Entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ") assigned ID: " + std::to_string(idIncr), "Arena::SetPixelWithId()");
        entityIndex[idIncr] = entity;
        entity->Id = idIncr;
        idIncr++;
        entity->SetPosition(p);
    }

    bool Arena::SetPixelWithIdSafe(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        util::WriteToLog("Attempting to set pixel safely and assign an ID at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")...", "Arena::SetPixelWithIdSafe()");
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            return false;
        }
        if (Entity::IsType(pixel[p.y][p.x], EntityType::AIR)) {
            delete pixel[p.y][p.x];
            pixel[p.y][p.x] = entity;
            util::WriteToLog("Entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ") assigned ID: " + std::to_string(idIncr), "Arena::SetPixelWithIdSafe()");
            entityIndex[idIncr] = entity;
            entity->Id = idIncr;
            idIncr++;
            entity->SetPosition(p);
            return true;
        }
        util::WriteToLog("Failed to set pixel at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ").", "Arena::SetPixelWithIdSafe()");
        return false;
    }

    Entity* Arena::GetPixelById(int id) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        auto it = entityIndex.find(id);
        if (it != entityIndex.end()) {
            return it->second;
        }
        // If not found, return nullptr
        return nullptr;
    }

    Game* Arena::GetGame() {
        std::lock_guard<std::mutex> lock(arenaMutex);
        return game;
    }

    void Arena::SetGame(Game* game) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        this->game = game;
    }

    void Arena::Replace(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        try {
            delete pixel[p.y][p.x];
        } catch(const std::exception& _) {
            ; // do nothing
        }
        pixel[p.y][p.x] = entity;
        entity->SetPosition(p);
    }

    void Arena::ReplaceWithId(int id, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        auto it = entityIndex.find(id);
        if (it != entityIndex.end()) {
            Point p = it->second->GetPosition();
            delete pixel[p.y][p.x];
            pixel[p.y][p.x] = entity;
            entity->SetPosition(p);
            entity->Id = id;
            entityIndex.erase(it);
            entityIndex[id] = entity;
        }
    }

    void Arena::Remove(Point p) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = new Air({p.x, p.y}, this);
    }

    void Arena::RemoveById(int id) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        auto it = entityIndex.find(id);
        if (it != entityIndex.end()) {
            Point p = it->second->GetPosition();
            delete pixel[p.y][p.x];
            pixel[p.y][p.x] = new Air({p.x, p.y}, this);
            entityIndex.erase(it);
        }
    }

    void Arena::Move(Point start, Point dest) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        delete pixel[dest.y][dest.x];
        pixel[dest.y][dest.x] = pixel[start.y][start.x];
        pixel[start.y][start.x] = new Air({start.x, start.y}, this);
        pixel[dest.y][dest.x]->SetPosition(dest);
    }

    std::vector<Entity*> Arena::GetMappedEntities() {
        std::lock_guard<std::mutex> lock(arenaMutex);
        std::vector<Entity*> entities;
        for (const auto& pair : entityIndex) {
            entities.push_back(pair.second);
        }
        return entities;
    }

    std::list<Entity*> Arena::GetEntitiesOfType(EntityType type) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        std::list<Entity*> entities;
        for (int y = 0; y < ARENA_HEIGHT; y++) {
            for (int x = 0; x < ARENA_WIDTH; x++) {
                if (Entity::IsType(pixel[y][x], type)) {
                    entities.push_back(pixel[y][x]);
                }
            }
        }
        return entities;
    }

}
