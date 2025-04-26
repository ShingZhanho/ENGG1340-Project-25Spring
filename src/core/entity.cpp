#include <core/entity.hpp>
#include <ftxui/screen/color.hpp>
#include <ui/common.hpp>

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
        return { // mobs with 1 HP should be italic, zombies are 1 HP so they are always italic
            'Z', ftxui::Color::Red, ftxui::Color::Default, true, true, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::TrollRenderOption() { 
        return {
            'T', ftxui::Color::Purple, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::BabyZombieRenderOption() {
        return {
            'z', ftxui::Color::Red, ftxui::Color::Default, true, true, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::MonsterRenderOption() {
        return {
            'M', ftxui::Color::Blue, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::BossRenderOption() {
        return {
            'B', ftxui::Color::Magenta, ftxui::Color::Default, true, false, false, true
        };
    }
    ui::RenderOption EntityRenderOptions::PlayerBulletRenderOption() { 
        return {
            '*', ftxui::Color::Yellow, ftxui::Color::Default, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::EnergyDrinkRenderOption(int hp) {
        return {
            static_cast<char>('0' + hp), ftxui::Color::DarkGreen, ftxui::Color::Green, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::StrengthPotionRenderOption(int damage) {
        return {
            static_cast<char>('0' + damage), ftxui::Color::DarkRed, ftxui::Color::Red, true, false, false, false
        };
    }
    ui::RenderOption EntityRenderOptions::ShieldRenderOption() {
        return {
            '|', ftxui::Color::Black, ftxui::Color::DarkCyan, true, false, false, false
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
                return true;
            case EntityType::ABSTRACT_BLOCK:
                return dynamic_cast<AbstractBlock*>(entity) != nullptr;
            case EntityType::ABSTRACT_MOB:
                return dynamic_cast<AbstractMob*>(entity) != nullptr;
            case EntityType::PLAYER_BULLET:
                return dynamic_cast<PlayerBullet*>(entity) != nullptr;
            case EntityType::ABSTRACT_COLLECTIBLE:
                return dynamic_cast<AbstractCollectible*>(entity) != nullptr;
            case EntityType::WALL:
                return dynamic_cast<Wall*>(entity) != nullptr;
            case EntityType::AIR:
                return dynamic_cast<Air*>(entity) != nullptr;
            case EntityType::PLAYER:
                return dynamic_cast<Player*>(entity) != nullptr;
            case EntityType::ZOMBIE:
                return dynamic_cast<Zombie*>(entity) != nullptr;
            case EntityType::TROLL:
                return dynamic_cast<Troll*>(entity) != nullptr;
            case EntityType::BABY_ZOMBIE:
                return dynamic_cast<BabyZombie*>(entity) != nullptr;
            case EntityType::MONSTER:
                return dynamic_cast<Monster*>(entity) != nullptr;
            case EntityType::BOSS:
                return dynamic_cast<Boss*>(entity) != nullptr;
            case EntityType::ENERGY_DRINK:
                return dynamic_cast<EnergyDrink*>(entity) != nullptr;
            case EntityType::STRENGTH_POTION:
                return dynamic_cast<StrengthPotion*>(entity) != nullptr;
            case EntityType::SHIELD:
                return dynamic_cast<Shield*>(entity) != nullptr;
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

    AbstractMob::AbstractMob(Point position, Arena* arena, int hp, int damage, int killScore, int ticksPerMove)
        : Entity(position, arena), hp(hp), damage(damage), killScore(killScore), ticksPerMove(ticksPerMove) {
            lastMoveTick = arena->GetGame()->GetGameClock();
        }

    int AbstractMob::GetHP() const { return hp; }

    void AbstractMob::TakeDamage(int damage) {
        if (shieldExpireTick < arena->GetGame()->GetGameClock()) {
            hp -= damage;
            renderOption.SetUnderline(false);
        }
        if (hp <= 1) renderOption.SetItalic(true); //  Set to italic when HP is low
        if (hp > 1) renderOption.SetItalic(false);
        // mob removal logic implemented in the event handler
    }

    void AbstractMob::ChangeDamage(int delta) {
        damage += delta;
    }

    bool AbstractMob::Move(Point to) {
        //  Check if move time has reached
        int currentTime = arena->GetGame()->GetGameClock();
        if (shieldExpireTick < currentTime) {
            renderOption.SetUnderline(false);
        }
        if (currentTime - lastMoveTick < ticksPerMove) {
            return false; // Not enough time has passed
        }

        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::PLAYER)) { // collides with player
            dynamic_cast<Player*>(target)->TakeDamage(damage);
            lastMoveTick = currentTime;
            return false; // Mob does not disappear after attack, it will stay and attack again until killed.
        }

        // collision with bullet will be handled in the bullet class

        if (IsType(target, EntityType::ABSTRACT_COLLECTIBLE)) { // collides with wall
            dynamic_cast<AbstractCollectible*>(target)->PickUp(this);
            lastMoveTick = currentTime;
            return false;
        }

        if (IsType(target, EntityType::AIR)) { // collides with air
            arena->Move(GetPosition(), to);
            lastMoveTick = currentTime;
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

    int AbstractMob::GetKillScore() const { return killScore; }

    void AbstractMob::ApplyShield(int duration) {
        shieldExpireTick = arena->GetGame()->GetGameClock() + duration;
        renderOption.SetUnderline(true);
    }

    //  END: AbstractMob

    //  BEGIN: AbstractCollectible

    AbstractCollectible::AbstractCollectible(Point position, Arena* arena, int lifetime) : Entity(position, arena), lifetime(lifetime) {
        spawnTick = arena->GetGame()->GetGameClock();
    }

    void AbstractCollectible::RefreshStatus() {
        long long currentTime = arena->GetGame()->GetGameClock();
        if (lifetime - (currentTime - spawnTick) <= 50 * 3) {
            //  When the collectible lifetime is less than 3 seconds, blink the collectible
            renderOption.SetBlink(true);
        }
    }

    bool AbstractCollectible::IsInvalid() const {
        if (pickedUp) return true; // picked up
        long long currentTime = arena->GetGame()->GetGameClock();
        if (currentTime - spawnTick > lifetime) {
            return true; // expired
        }
        return false; // still valid
    }

    bool AbstractCollectible::Move(Point p) {
        //  Collectibles cannot move.
        return false;
    }

    //  -- Implementation Classes -------------------------------------------------

    //  BEGIN: PlayerBullet

    PlayerBullet::PlayerBullet(Point position, Arena* arena, int damage, int direction)
        : Entity(position, arena), damage(damage), direction(direction), bulletSpawnTick(arena->GetGame()->GetGameClock()) { 
        renderOption = EntityRenderOptions::PlayerBulletRenderOption();
    }

    int PlayerBullet::GetDamage() const { return damage; }

    int PlayerBullet::GetDirection() const { return direction; }

    bool PlayerBullet::IsExploded() const { return exploded; }

    bool PlayerBullet::IsOnArena() const { return onArena; }

    bool PlayerBullet::Move(Point to) {
        //  check if bullet life time has reached
        long long currentTime = arena->GetGame()->GetGameClock();
        if (currentTime - bulletSpawnTick > 50 * 2) { // 2 seconds
            exploded = true; // bullet should be removed
            return false;
        }

        Entity* target = arena->GetPixel(to);

        if (IsType(target, EntityType::WALL)) {
            // reverses the direction of the bullet
            // the bullet will continue to move in the opposite direction
            if (direction == 0) direction = 4; // UP -> DOWN
            else if (direction == 1) direction = 5; // UP_LEFT -> DOWN_RIGHT
            else if (direction == 2) direction = 6; // LEFT -> RIGHT
            else if (direction == 3) direction = 7; // DOWN_LEFT -> UP_RIGHT
            else if (direction == 4) direction = 0; // DOWN -> UP
            else if (direction == 5) direction = 1; // DOWN_RIGHT -> UP_LEFT
            else if (direction == 6) direction = 2; // RIGHT -> LEFT
            else if (direction == 7) direction = 3; // UP_RIGHT -> DOWN_LEFT
            return false;
        }

        if (IsType(target, EntityType::PLAYER_BULLET)) {
            exploded = true; 
            return false;
        }

        if (IsType(target, EntityType::PLAYER)) {
            dynamic_cast<Player*>(target)->TakeDamage(damage);
            exploded = true;
            return false;
        }

        if (IsType(target, EntityType::ABSTRACT_MOB)) {
            dynamic_cast<AbstractMob*>(target)->TakeDamage(damage);
            exploded = true;
            return false;
        }

        if (IsType(target, EntityType::ABSTRACT_COLLECTIBLE)) {
            dynamic_cast<AbstractCollectible*>(target)->PickUp(this);
            exploded = true;
            return false;
        }

        if (IsType(target, EntityType::AIR)) {
            arena->Move(GetPosition(), to);
            return true;
        }
        return false;
    }

    bool PlayerBullet::Move() {
        //  Check if move time has reached
        long long currentTime = arena->GetGame()->GetGameClock();
        if (currentTime - lastMoveTick < 25) { // 2 moves per second
            return false;
        }
        //  Move the bullet to the next position based on its direction.
        Point nextPos = GetNextPosition();
        return Move(nextPos);
    }

    bool PlayerBullet::TryShoot() {
        //  The bullet entity will be created at the same position as the player,
        //  and will NOT be placed in the arena directly.
        Point targetPoint = GetNextPosition();
        Entity* target = arena->GetPixel(targetPoint);

        if (IsType(target, EntityType::ABSTRACT_MOB)) {
            dynamic_cast<AbstractMob*>(target)->TakeDamage(damage);
            exploded = true; // Bullet explodes on mob
            return false; // Bullet should be removed and never placed in the arena
        }

        if (IsType(target, EntityType::PLAYER)) {
            dynamic_cast<Player*>(target)->TakeDamage(damage);
            exploded = true; // Bullet explodes on player
            return false;
        }

        if (IsType(target, EntityType::WALL) || IsType(target, EntityType::PLAYER_BULLET)) {
            exploded = true; // Bullet explodes on wall or other bullet
            return false;
        }

        if (IsType(target, EntityType::ABSTRACT_COLLECTIBLE)) {
            //  The bullet will shatter the collectible, as if it was picked up
            dynamic_cast<AbstractCollectible*>(target)->PickUp(this);
            exploded = true; // Bullet explodes on collectible
            return false;
        }

        if (IsType(target, EntityType::AIR)) {
            //  the bullet will be placed in the arena
            arena->SetPixelSafe(targetPoint, this);
            onArena = true; // bullet is now on the arena
            return true;
        }

        // abnormal case - directly explode and remove
        exploded = true; // bullet explodes abnormally
        return false;
    }

    Point PlayerBullet::GetNextPosition() {
        Point nextPos = GetPosition();
        switch (direction) {
            case 0: // UP
                nextPos.y--;
                break;
            case 1: // UP_LEFT
                nextPos.x--;
                nextPos.y--;
                break;
            case 2: // LEFT
                nextPos.x--;
                break;
            case 3: // DOWN_LEFT
                nextPos.x--;
                nextPos.y++;
                break;
            case 4: // DOWN
                nextPos.y++;
                break;
            case 5: // DOWN_RIGHT
                nextPos.x++;
                nextPos.y++;
                break;
            case 6: // RIGHT
                nextPos.x++;
                break;
            case 7: // UP_RIGHT
                nextPos.x++;
                nextPos.y--;
                break;
            default:
                return nextPos; // Invalid direction, return current position
        }
        return nextPos; // Return the new position based on the direction
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
        damage = 1;
    }

    void Player::TakeDamage(int damage) {
        if (shieldExpireTick < arena->GetGame()->GetGameClock()) {
            hp -= damage;
            renderOption.SetUnderline(false);
        }
        if (hp <= 0) {
            ui::appScreen.ExitLoopClosure()();
            arena->GetGame()->Terminate();
        }
    }

    void Player::ChangeDamage(int delta) {
        damage += delta;
    }

    int Player::GetDamage() const {
        return damage;
    }

    bool Player::Move(Point to) {
        Entity* target = arena->GetPixel(to);

        if (shieldExpireTick < arena->GetGame()->GetGameClock()) {
            renderOption.SetUnderline(false);
        }

        if (IsType(target, EntityType::WALL) || IsType(target, EntityType::ABSTRACT_MOB)) {
            return false; // Cannot move into wall or mob
        }

        if (IsType(target, EntityType::ABSTRACT_COLLECTIBLE)) {
            dynamic_cast<AbstractCollectible*>(target)->PickUp(this);
            return false;
        }

        if (IsType(target, EntityType::AIR)) {
            arena->Move(GetPosition(), to);
            return true;
        }

        return false;
    }

    int Player::GetHP() const {
        return hp;
    }

    void Player::ApplyShield(int duration) {
        shieldExpireTick = arena->GetGame()->GetGameClock() + duration;
        renderOption.SetUnderline(true);
    }

    //  END: Player

    //  BEGIN: Zombie

    Zombie::Zombie(Point position, Arena* arena) 
        : AbstractMob(
            position, arena,
            1, 1, 1, 50 // HP, damage, killScore, ticksPerMove
        ) {
        renderOption = EntityRenderOptions::ZombieRenderOption();
    }

    //  END: Zombie
    
    //  BEGIN: Troll

    Troll::Troll(Point position, Arena* arena)
        : AbstractMob(
            position, arena, 
            5, 2, 5, 100 // HP, damage, killScore, ticksPerMove
        ) {
        renderOption = EntityRenderOptions::TrollRenderOption();
    }

    //  END: Troll

    //  BEGIN: BabyZombie

    BabyZombie::BabyZombie(Point position, Arena* arena)
        : AbstractMob(
            position, arena,
            1, 1, 2, 25 // HP, damage, killScore, ticksPerMove
        ) {
        renderOption = EntityRenderOptions::BabyZombieRenderOption();
    }

    //  END: BabyZombie

    //  BEGIN: Monster

    Monster::Monster(Point position, Arena* arena)
        : AbstractMob(
            position, arena,
            10, 5, 10, 25 // HP, damage, killScore, ticksPerMove
        ) {
        renderOption = EntityRenderOptions::MonsterRenderOption();
    }

    //  END: Monster

    //  BEGIN: Boss

    Boss::Boss(Point position, Arena* arena)
        : AbstractMob(
            position, arena,
            1000, 50, 100, 200 // HP, damage, killScore, ticksPerMove
        ) {
        renderOption = EntityRenderOptions::BossRenderOption();
    }

    //  END: Boss

    //  BEGIN: EnergyDrink

    EnergyDrink::EnergyDrink(Point position, Arena* arena, int healingPoint) : AbstractCollectible(position, arena, 50 * 10), hp(healingPoint) {
        renderOption = EntityRenderOptions::EnergyDrinkRenderOption(healingPoint);
    }

    bool EnergyDrink::PickUp(Entity* by) {
        if (IsType(by, EntityType::PLAYER)) {
            dynamic_cast<Player*>(by)->TakeDamage(-hp);
            pickedUp = true;
            return true;
        }

        if (IsType(by, EntityType::ABSTRACT_MOB)) {
            dynamic_cast<AbstractMob*>(by)->TakeDamage(-hp);
            pickedUp = true;
            return true;
        }

        if (IsType(by, EntityType::PLAYER_BULLET)) {
            pickedUp = true; // bullet will shatter the energy drink, as if it was picked up
            return true;
        }

        return false;
    }

    //  END: EnergyDrink

    //  BEGIN: StrengthPotion

    StrengthPotion::StrengthPotion(Point position, Arena* arena, int damage) : AbstractCollectible(position, arena, 50 * 10), damage(damage) {
        renderOption = EntityRenderOptions::StrengthPotionRenderOption(damage);
    }

    bool StrengthPotion::PickUp(Entity* by) {
        if (IsType(by, EntityType::PLAYER)) {
            dynamic_cast<Player*>(by)->ChangeDamage(damage);
            pickedUp = true;
            return true;
        }

        if (IsType(by, EntityType::ABSTRACT_MOB)) {
            dynamic_cast<AbstractMob*>(by)->ChangeDamage(damage);
            pickedUp = true;
            return true;
        }

        if (IsType(by, EntityType::PLAYER_BULLET)) {
            pickedUp = true; // bullet will shatter the strength potion, as if it was picked up
            return true;
        }

        return false;
    }

    int StrengthPotion::GetDamage() const {
        return damage;
    }

    //  END: StrengthPotion

    //  BEGIN: Shield

    Shield::Shield(Point position, Arena* arena, int duration) : AbstractCollectible(position, arena, 50 * 10), duration(duration) {
        renderOption = EntityRenderOptions::ShieldRenderOption();
    }

    bool Shield::PickUp(Entity* by) {
        if (IsType(by, EntityType::PLAYER)) {
            dynamic_cast<Player*>(by)->ApplyShield(duration);
            pickedUp = true;
            return true;
        }

        if (IsType(by, EntityType::ABSTRACT_MOB)) {
            dynamic_cast<AbstractMob*>(by)->ApplyShield(duration);
            pickedUp = true;
            return true;
        }

        if (IsType(by, EntityType::PLAYER_BULLET)) {
            pickedUp = true; // bullet will shatter the shield, as if it was picked up
            return true;
        }

        return false;
    }

    //  END: Shield
        
} // namespace Core
