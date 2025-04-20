#include <core/entity.hpp>
#include <ftxui/screen/color.hpp>
#include <util/log.hpp>

namespace core {

    //  BEGIN: EntityRenderOptions

    //  here defines the render options for each entity.
    //  The arguments are:
    //      1. Character to render - the character to display on the screen
    //      2. Foreground color - the color of the character
    //      3. Background color - the color of the background of the entity
    //      4. Bold - whether the character is bold
    //      5. Italic - whether the character is italic
    //      6. Underline - whether the character is underlined
    //      7. Blink - whether the character is blinking

    ui::RenderOption EntityRenderOptions::AirRenderOption() {
        return {};
    } // default options
    ui::RenderOption EntityRenderOptions::WallRenderOption() {
        return {
            'X', ftxui::Color::White, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::PlayerRenderOption() {
        return {
            '@', ftxui::Color::Green, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::ZombieRenderOption() {
        return {
            'Z', ftxui::Color::Red, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::TrollRenderOption() { 
        return {
            'T', ftxui::Color::Crimson, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::PlayerBulletRenderOption() { 
        return {
            '*', ftxui::Color::Yellow, ftxui::Color::Default, true, false, false, false
        };
    }
    

    //  END: EntityRenderOptions

    //  BEGIN: Entity

    Entity::Entity(Point position, Arena* arena) : position(position), arena(arena) {}

    Point Entity::GetPosition() {
        return position;
    }

    void Entity::SetPosition(Point position) {
        this->position = position;
    }

    bool Entity::IsType(Entity* entity, EntityType type) {
        if (entity == nullptr) return false;
        switch (type) {
            case EntityType::ABSTRACT_ENTITY:
                return dynamic_cast<Entity*>(entity) != nullptr;
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

    ui::RenderOption Entity::GetRenderOption() {
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
        util::WriteToLog("Attempting to move mob to: (" + std::to_string(to.x) + ", " + std::to_string(to.y) + ")", "AbstractMob::Move()");
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
            util::WriteToLog("Mob moved to: (" + std::to_string(to.x) + ", " + std::to_string(to.y) + ")", "AbstractMob::Move()");
            return true;
        }

        return false;
    }

    bool AbstractMob::Move() {
        //  Move towards the player using the given path.
        if (Path.empty()) {
            return false;
        } 
        Point nextPos = Path.front();
        if (Move(nextPos)) { // Try to move to the next position
            Path.pop_front();
            return true;
        }
        return false;
    }

    //  END: AbstractMob

    //  -- Implementation Classes -------------------------------------------------

    //  BEGIN: PlayerBullet

    PlayerBullet::PlayerBullet(Point position, Arena* arena, int damage, int direction)
        : Entity(position, arena), damage(damage), direction(direction) { 
        renderOption = EntityRenderOptions::PlayerBulletRenderOption();
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
        renderOption = EntityRenderOptions::WallRenderOption();
    }

    //  END: Wall

    //  BEGIN: Air

    Air::Air(Point position, Arena* arena) : AbstractBlock(position, arena) {
        renderOption = EntityRenderOptions::AirRenderOption();
    }

    //  END: Air

    //  BEGIN: Player

    Player::Player(Point position, Arena* arena, int initialHp) : Entity(position, arena), hp(initialHp) {
        renderOption = EntityRenderOptions::PlayerRenderOption();
    }

    void Player::TakeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) throw 0; // Player died
    }

    bool Player::Move(Point to) {
        util::WriteToLog("Attempting to move player to: (" + std::to_string(to.x) + ", " + std::to_string(to.y) + ")", "Player::Move()");
        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::WALL) || IsType(target, EntityType::ABSTRACT_MOB)) {
            util::WriteToLog("Cannot move into wall or mob. Request blocked.", "Player::Move()");
            return false; // Cannot move into wall or mob
        }

        if (IsType(target, EntityType::AIR)) {
            util::WriteToLog("Requesting arena to move Player entity...", "Player::Move()");
            arena->Move(GetPosition(), to);
            util::WriteToLog("Player moved to: (" + std::to_string(to.x) + ", " + std::to_string(to.y) + ")", "Player::Move()");
            return true;
        }

        return false;
    }

    int Player::GetHP() const {
        return hp;
    }

    //  END: Player

    //  BEGIN: Zombie

    Zombie::Zombie(Point position, Arena* arena) 
        : AbstractMob(
            position, arena,
            1, 1, 1 // HP, damage, killScore
        ) {
        renderOption = EntityRenderOptions::ZombieRenderOption();
    }

    //  END: Zombie
    
    //  BEGIN: Troll

    Troll:Troll(Point position, Arena* arena)
        : AbstractMob(
            position, arena, 
            5, 0.5, 5 // HP, damage, killScore
        ) {
        renderOption = EntityRenderOptions::TrollRenderOption():
    }

    //  END: Troll
        
} // namespace Core
