#ifndef CORE_ENTITY_TYPE_HPP
#define CORE_ENTITY_TYPE_HPP

namespace core {

    enum class EntityType {
        ABSTRACT_BLOCK, ABSTRACT_MOB, PLAYER_BULLET,
        WALL, AIR, PLAYER, ZOMBIE
    };

} // namespace core

#endif // CORE_ENTITY_TYPE_HPP