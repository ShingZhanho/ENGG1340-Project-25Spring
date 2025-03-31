//
// Created by Jacob Shing on 31/03/2025.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace core {

    typedef struct Point {
        int x;
        int y;
    } Point;

    class Entity {
        public:
            // Constructor
            Entity(int x, int y, char character);

            Point getPosition();
            //virtual bool move(Point to, Arena* arena) = 0;

        private:
            Point position;
            char character; // the appearance of the entity
    };

}

#endif //ENTITY_HPP
