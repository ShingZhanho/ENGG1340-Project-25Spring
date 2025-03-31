#ifndef ARENA_HPP
#define ARENA_HPP

#include <core/entity.hpp>

#define ARENA_WIDTH 102
#define ARENA_HEIGHT 32

namespace core {

    class Arena {
        public:
            // Constructors and destructors
            Arena();
            ~Arena();

            Entity* getPixel(Point p);
            void setPixel(Point p, Entity* entity);

        private:
            Entity* pixel[ARENA_HEIGHT][ARENA_WIDTH];
    };

}

#endif // ARENA_HPP