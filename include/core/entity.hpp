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
    class AbstractCollectible;
    class PlayerBullet;
    class Air;
    class Wall;
    class Player;
    class Zombie;
    class Troll;
    class BabyZombie;
    class Monster;
    class Boss;
    class StrengthPotion;
    class EnergyDrink;

    typedef EntityType EntityType;

    class EntityRenderOptions {
        public:
            static ui::RenderOption AirRenderOption();
            static ui::RenderOption WallRenderOption();
            static ui::RenderOption PlayerRenderOption();
            static ui::RenderOption ZombieRenderOption();
            static ui::RenderOption PlayerBulletRenderOption();
            static ui::RenderOption TrollRenderOption();
            static ui::RenderOption BabyZombieRenderOption();
            static ui::RenderOption MonsterRenderOption();
            static ui::RenderOption BossRenderOption();
            static ui::RenderOption EnergyDrinkRenderOption(int hp);
            static ui::RenderOption StrengthPotionRenderOption(int damage);
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
            //  The ID of the entity. For unmapped entities, the ID will be negative.
            int Id = -1;

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
            //  Applies damage to the mob. Apply negative damage to heal.
            void TakeDamage(int damage);
            //  Changes the damage value by delta.
            void ChangeDamage(int delta);
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
            //  Returns the kill score of the mob.
            int GetKillScore() const;

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
            long long lastMoveTick = -1;
    };

    //  Collectibles are entities that can be picked up by the player or mobs.
    //  The collectibles do not move and have limited lifetime. 
    class AbstractCollectible : public Entity {
        public:
            //  Constructor
            AbstractCollectible(Point position, Arena* arena, int lifetime);
            //  Let the given entity pick up the collectible.
            //  Returns true if the entity was able to pick up the collectible.
            virtual bool PickUp(Entity* by) = 0;
            //  Refreshes the status of the collectible.
            //  Should be called by dedicated event handler.
            void RefreshStatus();
            //  Determines if the collectible has expired/been picked up.
            bool IsInvalid() const;
            //  The override of the Move() method. Always returns false.
            bool Move(Point p) override;

        protected:
            //  Whether the collectible has been picked up.
            bool pickedUp = false;

        private:
            //  The tick when the collectible was spawned.
            long long spawnTick;
            //  The lifetime of the collectible in ticks.
            int lifetime;

    };

    //  -- Implementation Classes -------------------------------------------------

    class PlayerBullet : public Entity {
        public:
            PlayerBullet(Point position, Arena* arena, int damage, int direction);

            //  The bullet moves in the given direction. Should only be used internally.
            //  Call Move() in BulletMoveEventHandler to move the bullet.
            //  Call TryShoot() in PlayerShootEventHandler to shoot the bullet.
            bool Move(Point to) override;
            //  The Move() method dedicated to BulletMoveEventHandler.
            bool Move();
            //  Returns the damage of the bullet.
            int GetDamage() const;
            //  Returns the direction of the bullet.
            //  0 = UP, 1 = UP_LEFT, 2 = LEFT, 3 = DOWN_LEFT, 4 = DOWN, 5 = DOWN_RIGHT, 6 = RIGHT, 7 = UP_RIGHT
            int GetDirection() const;
            //  Attempts to shoot the bullet.
            //  Returns true if the bullet was placed in the arena successfully.
            //  Note that the bullet will not be placed in the arena if it directly collides with non-air entity,
            //  even if the entity took damage.
            bool TryShoot();
            //  Returns true if the bullet has exploded and should be removed.
            bool IsExploded() const;
            //  Returns true if the bullet is on the arena.
            bool IsOnArena() const;

        private:
            //  The damage of the bullet.
            int damage;
            //  The direction of the bullet.
            //  0 = UP, 1 = UP_LEFT, 2 = LEFT, 3 = DOWN_LEFT, 4 = DOWN, 5 = DOWN_RIGHT, 6 = RIGHT, 7 = UP_RIGHT
            int direction;
            //  Determines if the bullet has exploded and should be removed.
            bool exploded = false;
            //  Indicates if the bullet is on the arena.
            bool onArena = false;
            //  Gets the next position of the bullet based on the direction.
            Point GetNextPosition();
            //  The last tick when the bullet moved.
            long long lastMoveTick = -1;
            long long bulletSpawnTick;
    };

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
            //  Applies damage to the player. Apply negative damage to heal.
            void TakeDamage(int damage);
            //  Changes the damage value by delta.
            void ChangeDamage(int delta);
            //  Returns the damage of the player.
            int GetDamage() const;
            //  Moves the player to the given position.
            bool Move(Point to) override;
            //  Returns the health points of the player.
            int GetHP() const;

        private:
            //  The health points of the player.
            int hp;
            //  The damage of the player.
            int damage = 1;
    };

    class Zombie : public AbstractMob {
        public:
            Zombie(Point position, Arena* arena);
    };

    class Troll: public AbstractMob {
        public:
            Troll(Point position, Arena* arena);
    };

    class BabyZombie : public AbstractMob {
        public:
            BabyZombie(Point position, Arena* arena);
    };

    class Monster : public AbstractMob {
        public:
            Monster(Point position, Arena* arena);
    };

    class Boss : public AbstractMob {
        public:
            Boss(Point position, Arena* arena);
    };

    //  EnergyDrink is a tool that can be picked up by either the player or mobs.
    //  It restores the health points of the entity. It will disappear after a certain
    //  amount of time.
    class EnergyDrink: public AbstractCollectible{
        public:
            //  Constructor
            EnergyDrink(Point position, Arena* arena, int hp);
            //  Returns the health points of the energy drink.
            int GetHP() const;
            //  Let the given entity pick up the energy drink.
            bool PickUp(Entity* by) override;

        private:
            //  The health points of the energy drink. (1 - 9)
            int hp;
    };

    //  StrengthPotion is a tool that can be picked up by either the player or mobs.
    //  It increases the damage of the entity. It will disappear after a certain
    //  amount of time.
    class StrengthPotion: public AbstractCollectible{
        public:
            //  Constructor
            StrengthPotion(Point position, Arena* arena, int damage);
            //  Returns the damage of the strength potion.
            int GetDamage() const;
            //  Let the given entity pick up the strength potion.
            bool PickUp(Entity* by) override;

        private:
            //  The damage of the strength potion. (1 - 9)
            int damage;
    };
}

#endif // CORE_ENTITY_HPP
