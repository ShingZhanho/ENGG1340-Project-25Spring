#ifndef CORE_POINT_HPP
#define CORE_POINT_HPP

namespace core {

    typedef struct Point {
        int x;
        int y;
        bool operator==(const Point& other) const {
            return (x == other.x && y == other.y);
        };
        bool operator!=(const Point& other) const {
            return !(*this == other);
        };
        bool operator<(const Point& other) const {
            return (x < other.x || (x == other.x && y < other.y));
        };
    } Point;

}

namespace std {

    template <> struct hash<core::Point> {
        std::size_t operator()(const core::Point& point) const noexcept {
            return std::hash<int>()(point.x ^ (point.y << 16));
        }
    };

}

#endif // CORE_POINT_HPP