#ifndef CORE_POINT_HPP
#define CORE_POINT_HPP

namespace core {

    typedef struct Point {
        int x;
        int y;
        bool operator==(const Point& other) const {
            return (x == other.x && y == other.y);
        };
    } Point;

}

#endif // CORE_POINT_HPP