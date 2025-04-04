#ifndef CORE_ARENA_HPP
#define CORE_ARENA_HPP

#include <unordered_map>

#include <core/entity.hpp>

#define ARENA_WIDTH 102
#define ARENA_HEIGHT 32

namespace core {

    //  The arena. Every entity is placed inside.
    //  The Entity[32][102] Arena->pixel  is the core object of our game.
    //  This is NOT the output frame. It's the internal structured data.
    class Arena {
        public:
            // Constructors and destructors
            Arena();
            ~Arena();

            //  TODO: these two might be removed in the future.
            Entity* GetPixel(Point p);
            void SetPixel(Point p, Entity* entity);

            //  Replaces the pixel at (x, y) with the given entity.
            void Replace(Point p, Entity* entity);
            //  Removes the pixel at (x, y) and replaces it with air.
            void Remove(Point p);
            //  Moves the entity from one pixel to another.
            void Move(Point start, Point dest);

        private:
            //  A pixel is one single entity in the arena.
            //  Entity can be air, wall, player, mob, etc.
            Entity* pixel[ARENA_HEIGHT][ARENA_WIDTH];

            //  Used for efficiently searching through non-block entities.
            //  The id is incremented for each non-block entity created.
            int idIncr = 0;
            std::unordered_map<int, Entity*> entityIndex;
    };

}

#endif // ARENA_HPP