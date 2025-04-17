#include <core/entity.hpp>
#include <ftxui/screen/color.hpp>

namespace core {

    //  BEGIN: EntityRenderOptions

    //  here defines the render options for each entity.

    ui::RenderOption EntityRenderOptions::AirRenderOption = ui::RenderOption(); // default options
    ui::RenderOption EntityRenderOptions::WallRenderOption = ui::RenderOption(
        'X', ftxui::Color::GrayDark, ftxui::Color::Default, false, false, false, false
    );
    ui::RenderOption EntityRenderOptions::PlayerRenderOption = ui::RenderOption(
        '@', ftxui::Color::Green, ftxui::Color::Default, true, false, false, false
    );
    ui::RenderOption EntityRenderOptions::ZombieRenderOption = ui::RenderOption(
        'Z', ftxui::Color::Red, ftxui::Color::Default, true, false, false, false
    );
    ui::RenderOption EntityRenderOptions::PlayerBulletRenderOption = ui::RenderOption(
        '*', ftxui::Color::Yellow, ftxui::Color::Default, true, false, false, false
    );

    //  END: EntityRenderOptions

    //  BEGIN: Entity

    Entity::Entity(Point position, Arena* arena) : position(position), arena(arena) {}

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
            case EntityType::PLAYER_BULLET:
                return dynamic_cast<PlayerBullet*>(entity) != nullptr;
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

    ui::RenderOption* Entity::GetRenderOption() {
        return renderOption;
    }

    //  END: Entity

    //  -- Abstract Classes ---------------------------------------------------------

    //  BEGIN: AbstractBlock

    AbstractBlock::AbstractBlock(Point position, Arena* arena) : Entity(position, arena) {}

    bool AbstractBlock::Move(Point to) {
        //  Blocks cannot move.
        return false;
    }

    //  END: AbstractBlock

    //  BEGIN: AbstractMob

    AbstractMob::AbstractMob(Point position, Arena* arena, int hp, int damage, int killScore)
        : Entity(position, arena), hp(hp), damage(damage), killScore(killScore) {}

    int AbstractMob::GetHP() const { return hp; }

    void AbstractMob::TakeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) {
            arena->GetGame()->ChangeScore(killScore);
            arena->Remove(GetPosition());
        }
    }

    bool AbstractMob::Move(Point to) {
        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::PLAYER)) { // collides with player
            dynamic_cast<Player*>(target)->TakeDamage(damage);
            return false; // Mob does not disappear after attack, it will stay and attack again until killed.
        }

        if (IsType(target, EntityType::PLAYER_BULLET)) { // collides with bullet
            //  Note: may change to type PLAYER_BULLET in the future if mobs can fire bullets.

            int damage = dynamic_cast<PlayerBullet*>(target)->GetDamage();
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

    //  END: AbstractMob

    //  -- Implementation Classes -------------------------------------------------

    //  BEGIN: PlayerBullet

    PlayerBullet::PlayerBullet(Point position, Arena* arena, int damage, int direction)
        : Entity(position, arena), damage(damage), direction(direction) {
        Entity::renderOption = &EntityRenderOptions::PlayerBulletRenderOption;
    }

    int PlayerBullet::GetDamage() const { return damage; }

    int PlayerBullet::GetDirection() const { return direction; }

    bool PlayerBullet::Move(Point to) {
        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::WALL) 
            || IsType(target, EntityType::PLAYER_BULLET)
            || IsType(target, EntityType::PLAYER)) {
            arena->Remove(GetPosition());
            return false;
        }

        if (IsType(target, EntityType::ABSTRACT_MOB)) {
            dynamic_cast<AbstractMob*>(target)->TakeDamage(damage);
            arena->Remove(GetPosition());
            return false;
        }

        if (IsType(target, EntityType::AIR)) {
            arena->Move(GetPosition(), to);
            return true;
        }
        return false;
    }

    //  END: PlayerBullet

    //  BEGIN: Wall

    Wall::Wall(Point position, Arena* arena) : AbstractBlock(position, arena) {
        Entity::renderOption = &EntityRenderOptions::WallRenderOption;
    }

    //  END: Wall

    //  BEGIN: Air

    Air::Air(Point position, Arena* arena) : AbstractBlock(position, arena) {
        Entity::renderOption = &EntityRenderOptions::AirRenderOption;
    }

    //  END: Air

    //  BEGIN: Player

    Player::Player(Point position, Arena* arena) : Entity(position, arena) {
        Entity::renderOption = &EntityRenderOptions::PlayerRenderOption;
        // set hp to the initial value from GameOptions
        hp = arena->GetGame()->GetOptions()->PlayerHp;
    }

    void Player::TakeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) throw 0; // Player died
    }

    bool Player::Move(Point to) {
        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::WALL) || IsType(target, EntityType::ABSTRACT_MOB)) {
            return false; // Cannot move into wall or mob
        }

        if (IsType(target, EntityType::AIR)) {
            arena->Move(GetPosition(), to);
            return true;
        }

        return false;
    }

    //  END: Player

    //  BEGIN: Zombie

    Zombie::Zombie(Point position, Arena* arena) 
        : AbstractMob(
            position, arena,
            1, 1, 1 // HP, damage, killScore
        ) {
        Entity::renderOption = &EntityRenderOptions::ZombieRenderOption;
    }

    //  END: Zombie
    
} // namespace Core
