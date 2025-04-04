#include <core/entity.hpp>

namespace core {

    Entity::Entity(int x, int y, char character) : position({x, y}), character(character) {}

    Point Entity::GetPosition() {
        return position;
    }

    void Entity::SetPosition(Point position) {
        position = position;
    }
    
} // namespace Core
