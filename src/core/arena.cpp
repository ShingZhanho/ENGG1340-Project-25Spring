#include <core/arena.hpp>
#include <core/entity.hpp>

namespace core {

    Arena::Arena() {
        for (int i = 0; i < ARENA_HEIGHT; i++) {
            for (int j = 0; j < ARENA_WIDTH; j++) {
                pixel[i][j] = new Air({i, j}, this);
            }
        }
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
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = entity;
    }

    void Arena::SetPixelWithId(Point p, Entity* entity) {
        //  TODO: implement this method
        //  Steps:
        //      1. Call SetPixel() with the given parameters.
        //      2. In the object std::unordered_map<int, Entity*> entityIndex,
        //          add the entity with the idIncr as the key and the entity pointer as the value.
        //      3. Increment idIncr.
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

    void Arena::Remove(Point p) {
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = new Air({p.x, p.y}, this);
    }

    void Arena::Move(Point start, Point dest) {
        Replace(dest, pixel[start.y][start.x]);
        Remove(start);
    }

}