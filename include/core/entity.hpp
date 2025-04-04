//
// Created by Jacob Shing on 31/03/2025.
//

#ifndef CORE_ENTITY_HPP
#define CORE_ENTITY_HPP

#include <core/arena.hpp>
#include <ui/render_option.hpp>

namespace core {

    // Forward declarations
    class Arena;

    class AbstractBlock;
    class AbstractMob;
    class AbstractBullet;
    class Air;
    class Wall;
    class Player;
    class Zombie;

    typedef struct Point {
        int x;
        int y;
    } Point;

    class Entity {
        public:
            // Constructor
            Entity(Point position, char character, Arena* arena);
            virtual ~Entity() = default;

            Point GetPosition();
            //  Only used within the class Arena. Use Move() in game loop to handle
            //  the movement of the entity.
            void SetPosition(Point position);
            virtual bool Move(Point to) = 0;

        protected:
            //  Used internally for checking the type of entity.
            //  Update this enum when adding new entity types.
            enum class EntityType {
                ABSTRACT_BLOCK, ABSTRACT_MOB, ABSTRACT_BULLET,
                WALL, AIR, PLAYER, ZOMBIE
            };

            //  Returns true if the entity is of the given type.
            //  This is a wrapper for dynamic_cast.
            static bool IsType(Entity* entity, EntityType type);

            //  A link to the arena.
            Arena* arena;

        private:
            //  The ID. Non-block entity will have an ID.
            int id;
            Point position;
            char character; // the appearance of the entity; TODO: replace with RenderOption
    };

    //  -- Abstract Classes ---------------------------------------------------------

    //  Blocks are fixed entities in the arena that do not move and prevent
    //  other entities from moving to their position.
    class AbstractBlock : public Entity {
        public:
            AbstractBlock(Point position, Arena* arena);
            virtual ~AbstractBlock() = default;
    };

    //  Mobs are entities that can move and attack.
    //  Mobs move towards the player using A* algorithm.
    class AbstractMob : public Entity {
        public:
            AbstractMob(Point position, Arena* arena, int hp, int damage, int killScore);
            virtual ~AbstractMob() = default;

            int GetHP() const;
            //  Applies damage to the mob.
            void TakeDamage(int damage);
            bool Move(Point to) override;

        private:
            int hp;
            int damage;
            //  Scores earned when the mob is killed.
            int killScore;
    };
}

#endif //ENTITY_HPP
