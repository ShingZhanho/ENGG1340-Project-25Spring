//
// Created by Jacob Shing on 31/03/2025.
//

#ifndef CORE_ENTITY_HPP
#define CORE_ENTITY_HPP

#include <ui/render_option.hpp>

namespace core {

    // Forward declarations
    class Arena;

    class Air;
    class Wall;
    class MobWall;

    typedef struct Point {
        int x;
        int y;
    } Point;

    class Entity {
        public:
            // Constructor
            Entity(int x, int y, char character);
            virtual ~Entity() = default;

            Point getPosition();
            virtual bool move(Point to, Arena* arena) = 0;

        private:
            Point position;
            char character; // the appearance of the entity
    };

    class Air : public Entity {
        public:
            Air(int x, int y) : Entity(x, y, ' ') {}

            bool move(Point to, Arena* arena) override {
                return false;
            }
    };

    class Wall : public Entity {
        public:
            Wall(int x, int y) : Entity(x, y, 'X') {}

            bool move(Point to, Arena* arena) override {
                return false;
            }
    };

    class MobWall : public Entity {
        public:
            MobWall(int x, int y) : Entity(x, y, 'M') {}

            bool move(Point to, Arena* arena) override {
                return false;
            }
    };

}

#endif //ENTITY_HPP
