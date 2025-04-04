#include <core/entity.hpp>

namespace core {

    //  BEGIN: Entity

    Entity::Entity(Point position, char character, Arena* arena) 
        : position(position), character(character), arena(arena) {}

    Point Entity::GetPosition() {
        return position;
    }

    void Entity::SetPosition(Point position) {
        position = position;
    }

    bool Entity::IsType(Entity* entity, EntityType type) {
        switch (type) {
            case EntityType::ABSTRACT_BLOCK:
                return dynamic_cast<AbstractBlock*>(entity) != nullptr;
            case EntityType::ABSTRACT_MOB:
                return dynamic_cast<AbstractMob*>(entity) != nullptr;
            case EntityType::ABSTRACT_BULLET:
                return dynamic_cast<AbstractBullet*>(entity) != nullptr;
            case EntityType::WALL:
                return dynamic_cast<Wall*>(entity) != nullptr;
            case EntityType::AIR:
                return dynamic_cast<Air*>(entity) != nullptr;
            case EntityType::PLAYER:
                return dynamic_cast<Player*>(entity) != nullptr;
            case EntityType::ZOMBIE:
                return dynamic_cast<Zombie*>(entity) != nullptr;
            default:
                return false;
        }
    }

    //  END: Entity

    //  -- Abstract Classes ---------------------------------------------------------

    //  BEGIN: AbstractMob

    AbstractMob::AbstractMob(Point position, Arena* arena, int hp, int damage, int killScore)
        : Entity(position, 'M', arena), hp(hp), damage(damage), killScore(killScore) {}

    int AbstractMob::GetHP() const { return hp; }

    void AbstractMob::TakeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) {
            arena->Game()->ChangeScore(killScore);
            arena->Remove(GetPosition());
        }
    }

    bool AbstractMob::Move(Point to) {
        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::PLAYER)) { // collides with player
            dynamic_cast<Player*>(target)->TakeDamage(damage);
            return false; // Mob does not disappear after attack, it will stay and attack again until killed.
        }

        if (IsType(target, EntityType::ABSTRACT_BULLET)) { // collides with bullet
            //  Note: may change to type PLAYER_BULLET in the future if mobs can fire bullets.

            int damage = dynamic_cast<AbstractBullet*>(target)->GetDamage();
            arena->Move(GetPosition(), to);
            TakeDamage(damage);
            return true;
        }

        if (IsType(target, EntityType::AIR)) { // collides with air
            arena->Move(GetPosition(), to);
            return true;
        }

        return false;
    }
    
} // namespace Core
