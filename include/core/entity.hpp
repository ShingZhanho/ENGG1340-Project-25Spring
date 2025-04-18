#ifndef CORE_ENTITY_HPP
#define CORE_ENTITY_HPP

#include <core/arena.hpp>
#include <core/entity_type.hpp>
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
    typedef EntityType EntityType;

    class EntityRenderOptions {
        public:
            static ui::RenderOption AirRenderOption();
            static ui::RenderOption WallRenderOption();
            static ui::RenderOption PlayerRenderOption();
            static ui::RenderOption ZombieRenderOption();
            static ui::RenderOption PlayerBulletRenderOption();
    };

    class Entity {
        public:
            // Constructor
            Entity(Point position, Arena* arena);
            virtual ~Entity() = default;

            Point GetPosition();
            //  Only used within the class Arena. Use Move() in game loop to handle
            //  the movement of the entity.
            void SetPosition(Point position);
            virtual bool Move(Point to) = 0;
            //  Returns true if the entity is of the given type.
            //  This is a wrapper for dynamic_cast.
            static bool IsType(Entity* entity, EntityType type);
            //  Returns the render option of the entity
            ui::RenderOption GetRenderOption();

        protected:

            //  A link to the arena.
            Arena* arena;

            //  The render option for the entity. Must be manually set in the constructor.
            ui::RenderOption renderOption;

        private:
            //  The ID. Non-block entity will have an ID.
            int id;
            Point position;
    };

    //  -- Abstract Classes ---------------------------------------------------------

    //  Blocks are fixed entities in the arena that do not move and prevent
    //  other entities from moving to their position.
    class AbstractBlock : public Entity {
        public:
            AbstractBlock(Point position, Arena* arena);
            virtual ~AbstractBlock() = default;

            bool Move(Point to) override;
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

    class PlayerBullet : public Entity {
        public:
            PlayerBullet(Point position, Arena* arena, int damage, int direction);

            //  The bullet moves in the given direction.
            bool Move(Point to) override;

            //  Returns the damage of the bullet.
            int GetDamage() const;
            //  Returns the direction of the bullet.
            int GetDirection() const;

        private:
            //  The damage of the bullet.
            int damage;
            //  The direction of the bullet. 0 = up, 1 = right, 2 = down, 3 = left.
            int direction;
    };

    //  -- Implementation Classes -------------------------------------------------

    class Wall : public AbstractBlock {
        public:
            Wall(Point position, Arena* arena);
    };

    class Air : public AbstractBlock {
        public:
            Air(Point position, Arena* arena);
    };

    class Player : public Entity {
        public:
            Player(Point position, Arena* arena, int initialHp);
            
            void TakeDamage(int damage);
            bool Move(Point to) override;

        private:
            //  The health points of the player.
            int hp;
    };

    class Zombie : public AbstractMob {
        public:
            Zombie(Point position, Arena* arena);
    };
}

#endif // CORE_ENTITY_HPP
