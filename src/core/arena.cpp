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
        for (int i = 0; i < ARENA_HEIGHT; i++) {
            for (int j = 0; j < ARENA_WIDTH; j++) {
                delete pixel[i][j];
                pixel[i][j] = nullptr;
            }
        }
    }

    Entity* Arena::GetPixel(Point p) {
        return pixel[p.y][p.x];
    }

    void Arena::SetPixel(Point p, Entity* entity) {
        util::WriteToLog("Setting pixel at (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")", "Arena::SetPixel()");
        if (p.x == 0 || p.x == ARENA_WIDTH - 1 || p.y == 0 || p.y == ARENA_HEIGHT - 1) {
            // Do not allow setting pixels on the outermost layer
            util::WriteToLog("Attempted to set pixel on the outermost layer. Request Ignored.", "Arena::SetPixel()", "WARNING");
            return;
        }
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = entity;
    }

    void Arena::SetPixelWithId(Point p, Entity* entity) {
        SetPixel(p, entity);
        util::WriteToLog("Entity assigned ID: " + std::to_string(idIncr), "Arena::SetPixelWithId()");
        entityIndex[idIncr] = entity;
        idIncr++;
    }

    Entity* Arena::GetPixelById(int id) {
        auto it = entityIndex.find(id);
        if (it != entityIndex.end()) {
            return it->second;
        }
        return nullptr;
    }

    Game* Arena::GetGame() {
        return game;
    }

    void Arena::Replace(Point p, Entity* entity) {
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = entity;
        entity->SetPosition(p);
    }

    void Arena::ReplaceWithId(int id, Entity* entity) {
        auto it = entityIndex.find(id);
        if (it != entityIndex.end()) {
            Point p = it->second->GetPosition();
            delete pixel[p.y][p.x];
            pixel[p.y][p.x] = entity;
            entity->SetPosition(p);
            entityIndex.erase(it);
            entityIndex[id] = entity;
        }
    }

    void Arena::Remove(Point p) {
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = new Air({p.x, p.y}, this);
    }

    void Arena::Move(Point start, Point dest) {
        Replace(dest, pixel[start.y][start.x]);
        Remove(start);
    }

}
