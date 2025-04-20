#ifndef CORE_ENTITY_HPP
#define CORE_ENTITY_HPP

#include <list>

#include <core/arena.hpp>
#include <core/point.hpp>
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
    class Troll;

    typedef EntityType EntityType;

    class EntityRenderOptions {
        public:
            static ui::RenderOption AirRenderOption();
            static ui::RenderOption WallRenderOption();
            static ui::RenderOption PlayerRenderOption();
            static ui::RenderOption ZombieRenderOption();
            static ui::RenderOption PlayerBulletRenderOption();
            static ui::RenderOption TrollRenderOption();
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
            AbstractMob(Point position, Arena* arena, int hp, int damage, int killScore, int ticksPerMove);
            virtual ~AbstractMob() = default;

            int GetHP() const;
            //  Applies damage to the mob.
            void TakeDamage(int damage);
            //  Moves the mob to the given position.
            //  Returns true if the mob was able to move.
            //  Only used internally in the AbstractMob class.
            //  Please call Move() that takes no arguments to move the mob along its path.
            bool Move(Point to) override;
            //  Moves the mob along its path.
            //  This is a dedicated function that is called by the game loop.
            bool Move();
            //  The path towards the player. Updated through MobMoveEventHandler.
            std::list<Point> Path;

        private:
            int hp;
            int damage;
            //  Scores earned when the mob is killed.
            int killScore;
            //  Determines the speed of the mob, in ticks per move.
            //  The game runs at 50 ticks per second (i.e. 20ms per tick).
            //  tickPerMove = 50 means the mob moves one step every second.
            int ticksPerMove;
            //  The moment when the mob last moved.
            int lastMoveTick = -1;
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
            //  Returns the health points of the player.
            int GetHP() const;

        private:
            //  The health points of the player.
            int hp;
    };

    class Zombie : public AbstractMob {
        public:
            Zombie(Point position, Arena* arena);
    };

    class Troll: public AbstractMob {
        public:
            Troll(Point position, Arena* arena);
    };
}

#endif // CORE_ENTITY_HPP
