#include <core/arena.hpp>

namespace core {

    Arena::Arena() {
        for (int i = 0; i < ARENA_HEIGHT; i++) {
            for (int j = 0; j < ARENA_WIDTH; j++) {
                pixel[i][j] = new Air(i, j);
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

    Entity* Arena::getPixel(Point p) {
        return pixel[p.y][p.x];
    }

    void Arena::setPixel(Point p, Entity* entity) {
        delete pixel[p.y][p.x];
        pixel[p.y][p.x] = entity;
    }

}