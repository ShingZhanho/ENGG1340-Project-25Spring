#ifndef CORE_ARENA_HPP
#define CORE_ARENA_HPP

#include <unordered_map>

#include <core/game.hpp>
#include <core/entity.hpp>

#define ARENA_WIDTH 102
#define ARENA_HEIGHT 32

namespace core {

    //  Forward declarations
    class Game;
    class Entity;
    struct Point;

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
            Entity* GetPixelById(int id);

            Game* GetGame();

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
            Game* game;
    };

    //  This 32 * 102 is the exact size of the arena:
    //          
    //       Y
    //       0         1         2         3         4         5         6         7         8         9         0
    //       012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901
    //  X 0 0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //      1X -                                                                                                  X
    //      2X|+-------------------------------------------------------------------------------------------------|X
    //      3X |                                            100 wall
    //      4X |
    //      5X |
    //      6X |
    //      7X |
    //      8X |
    //      9X |
    //    1 0X |
    //      1X |
    //      2X |
    //      3X |
    //      4X |
    //      5X | 30 Wall
    //      6X |
    //      7X |
    //      8X |
    //      9X |
    //    2 0X |
    //      1X |
    //      2X |
    //      3X |
    //      4X |
    //      5X |
    //      6X |
    //      7X |
    //      8X |
    //      9X |
    //    3 0X -
    //      1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

}

#endif // CORE_ARENA_HPP