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
                try {
                    delete pixel[i][j];
                    pixel[i][j] = nullptr;
                } catch(const std::exception& _) {
                    ; // do nothing
                }
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
        try {
            delete pixel[p.y][p.x];
        } catch(const std::exception& _) {
            ; // do nothing
        }
        pixel[p.y][p.x] = entity;
        entity->SetPosition(p);
    }

    bool Arena::SetPixelSafe(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        util::WriteToLog("Attempting to set pixel safely at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")...", "Arena::SetPixelSafe()");
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            return false;
        }
        if (Entity::IsType(pixel[p.y][p.x], EntityType::AIR)) {
            try {
                delete pixel[p.y][p.x];
            } catch(const std::exception& _) {
                ; // do nothing
            }
            pixel[p.y][p.x] = entity;
            entity->SetPosition(p);
            util::WriteToLog("Pixel set successfully at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ").", "Arena::SetPixelSafe()");
            return true;
        }
        util::WriteToLog("Failed to set pixel at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ").", "Arena::SetPixelSafe()");
        return false;
    }

    void Arena::SetPixelWithId(Point p, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        util::WriteToLog("Attempting to set pixel and assign an ID at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")...", "Arena::SetPixelWithId()");
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            return;
        }
        try {
            delete pixel[p.y][p.x];
        } catch(const std::exception& _) {
            ; // do nothing
        }
        pixel[p.y][p.x] = entity;
        util::WriteToLog("Entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ") assigned ID: " + std::to_string(idIncr), "Arena::SetPixelWithId()");
        entityIndex[idIncr++] = entity;
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
            try {
                delete pixel[p.y][p.x];
            } catch(const std::exception& _) {
                ; // do nothing
            }
            pixel[p.y][p.x] = entity;
            util::WriteToLog("Entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ") assigned ID: " + std::to_string(idIncr), "Arena::SetPixelWithIdSafe()");
            entityIndex[idIncr++] = entity;
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
        util::WriteToLog("Entity with ID: " + std::to_string(id) + " not found.", "Arena::GetPixelById()");
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
        util::WriteToLog("Attempting to replace entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")...", "Arena::Replace()");
        try {
            delete pixel[p.y][p.x];
        } catch(const std::exception& _) {
            ; // do nothing
        }
        pixel[p.y][p.x] = entity;
        entity->SetPosition(p);
        util::WriteToLog("Entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ") replaced successfully.", "Arena::Replace()");
    }

    void Arena::ReplaceWithId(int id, Entity* entity) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        auto it = entityIndex.find(id);
        if (it != entityIndex.end()) {
            Point p = it->second->GetPosition();
            try {
                delete pixel[p.y][p.x];
            } catch(const std::exception& _) {
                ; // do nothing
            }
            pixel[p.y][p.x] = entity;
            entity->SetPosition(p);
            entityIndex.erase(it);
            entityIndex[id] = entity;
        }
    }

    void Arena::Remove(Point p) {
        std::lock_guard<std::mutex> lock(arenaMutex);
        util::WriteToLog("Attempting to remove entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")...", "Arena::Remove()");
        try {
            delete pixel[p.y][p.x];
        } catch(const std::exception& _) {
            ; // do nothing
        }
        pixel[p.y][p.x] = new Air({p.x, p.y}, this);
        util::WriteToLog("Entity at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ") removed successfully.", "Arena::Remove()");
    }

    void Arena::Move(Point start, Point dest) {
        util::WriteToLog("Attempting to move entity from (" + std::to_string(start.x) + ", " + std::to_string(start.y) + ") to (" + std::to_string(dest.x) + ", " + std::to_string(dest.y) + ")...", "Arena::Move()");
        std::lock_guard<std::mutex> lock(arenaMutex);
        try {
            delete pixel[dest.y][dest.x];
        } catch(const std::exception& _) {
            ; // do nothing
        }
        pixel[dest.y][dest.x] = pixel[start.y][start.x];
        pixel[start.y][start.x] = new Air({start.x, start.y}, this);
        pixel[dest.y][dest.x]->SetPosition(dest);
        util::WriteToLog("Entity moved from (" + std::to_string(start.x) + ", " + std::to_string(start.y) + ") to (" + std::to_string(dest.x) + ", " + std::to_string(dest.y) + ") successfully.", "Arena::Move()");
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
