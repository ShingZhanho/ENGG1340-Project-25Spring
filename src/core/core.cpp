//  ASCII-only is recommended for compatibility.
//  All comments begin with // then 2 spaces. In total that's 4 character, the length of a typical tab.
//  YOU MUST COMMENT YOUR CODE.


//  Some general C++ tips:
//  1.  Use '' for char. "" is char*.
//  2.  Remember to delete your newed objects. A common practice is do cascadely delete all newed childs where it is declared, often in the destructor.
//      Write delete right after you write new to prevent memory leak.


//  Basic data structure
#include <vector>
#include <unordered_map>
//  sleep_for() & now() / millisecond() support. Used for controlling fps.
#include <chrono>
#include <thread>

using namespace std;
namespace core {
    //  The main object representing the whole round.
    class Game {
        public:
        //  Variable declaration.
        //  The score. Initial score is 0.
        int score;
        //  The arena. refer to class definition for detail.
        //  The Data (Arena + Entity) and Instruction (Game + EventHandler) relationship is the foundational architecture for the game.
        Arena* arena;
        //  The root event.
        RunEventHandler* runEventHandler;
        //  Game setings.
        //  Currently empty: add stage number, difficulty settings here

        //  Constructor to set the game args
        Game() {

        }

        //  The entry point. A game is started by new Game().run()  
        int run() {
            //  Run the game.
            try {
                this->runEventHandler = new RunEventHandler(this);
                runEventHandler->fire();
            }
            //  The game ends when a error int endType is thrown.
            //  endType 0: Player died.
            catch (int endType) {
                return score;
            }
        }
        //  Cascade destruct.
        ~Game() {
            delete runEventHandler;
            //  Notice that the arena is deleted where it is newed.
            //  The game only manages runEventHandler. Other delete statements should be in EventHandler destructors.
            //  (Unless you new some objects in the Game itself)
        }
    };

    //  The arena. Every entity is placed inside.
    //  The Entity[32][102] Arena->pixel  is the core object of our game.
    //  This is NOT the output frame. It's the internal structured data.
    //
    //  This 32 * 102 is the exact size of the arena:
    //          
    //       Y
    //       0         1         2         3         4         5         6         7         8         9         0
    //       012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901
    //  X 0 0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //      1X -                                                                                                  X
    //      2X|+-------------------------------------------------------------------------------------------------|X
    //      3X |                                            100 wall
    //      4X |
    //      5X |
    //      6X |
    //      7X |
    //      8X |
    //      9X |
    //    1 0X |
    //      1X |
    //      2X |
    //      3X |
    //      4X |
    //      5X | 30 Wall
    //      6X |
    //      7X |
    //      8X |
    //      9X |
    //    2 0X |
    //      1X |
    //      2X |
    //      3X |
    //      4X |
    //      5X |
    //      6X |
    //      7X |
    //      8X |
    //      9X |
    //    3 0X -
    //      1XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    //        
    //  The character - object type map:
    //      
    //        -> Empty position. Notice that the character is space ' ' not empty ''.
    //      X -> Edge wall.
    //
    //      @ -> Player.
    //      . -> Player bullet.
    //
    //      o -> Mob. Zombies.
    //
    class Arena {
        public:
        //  A link to the game
        Game* game;
        //  An id system is implemented to identify and efficiently search through all existing entities THAT ARE NOT BLOCKS (see below -> entities).
        //  Constructing entity automatically assigns it with an auto increment id and associate it with the entityIndex.
        //  Destructing entity automatically removes it from the index.
        int id_incr;
        unordered_map<int, Entity*> entityIndex;
        //  'Pixel' is the name for the place, block... In this project, we're calling it pixel. We need to unify this name.
        //  Use methods. Do not assign pixel directly otherwise you're likely to forget deleting the old entities.
        Entity* pixel[32][102];
        Arena(Game* game) {
            this->game = game;
            this->id_incr = 0;
            //  Construct the pixels. Nothing is placed inside except air.
            for (int i = 0; i < 32; i++) {
                for (int j = 0; j < 32; j++) {
                    this->pixel[i][j] = new Air(i, j, this);
                }
            }
        }
        //  Replace method deletes the existing entity at (x, y), then associate the position with the new given entity.
        //  It also rewrites the x, y of moved entities.
        void replace(int x, int y, Entity* entity) {
            delete this->pixel[x][y];
            this->pixel[x][y] = entity;
            entity->x = x;
            entity->y = y;
        }
        //  Replaces given pixel with new Air.
        void remove(int x, int y) {
            delete this->pixel[x][y];
            this->pixel[x][y] = new Air(x, y, this);
        }
        //  Move method replaces the destination with starting point entity, then replace the starting point entity with new air.
        void move(int startingX, int startingY, int destinationX, int destinationY) {
            this->replace(destinationX, destinationY, this->pixel[startingX][startingY]);
            this->replace(startingX, startingY, new Air(startingX, startingY, this));
        }
        //  Destructs all alive entities.
        ~Arena() {
            for (int i = 0; i < 32; i++) {
                for (int j = 0; j < 32; j++) {
                    delete this->pixel[i][j];
                }
            }
        }
    };


    //  The parent class of all entities: wall, player, mob, bullets, air, etc. Anything in the arena counts, even air.
    //  Inheritance tree (Add your new implementations here):
    //
    //  (Entity) -----+-- Player
    //                |
    //                +-- PlayerBullet
    //                |
    //                +-- (Block) --+-- Air 
    //                |             |
    //                |             +-- Wall
    //                |
    //                +-- (Mob) --+-- Zombie
    //
    //  () stands for abstract class.
    // 
    //  Below, abstract classes are defined first, then actual classes.
    //

    class Entity {
        public:
        //  Basic fields. These fields should be filled in subclass constructor.
        //  The id. Non-block entity constructed has an id and can be searched from the arena.entityIndex efficiently.
        //  For simplicity the id system is defined within the Entity class. The Block id is not initialized. Be careful!
        int id;
        //  The position (x, y).
        //  Any entity must occupy exact 1 position in the arena.
        int x;
        int y;
        //  The output character. This is a class member, same for all instance.
        //  All entity class must set its character in the constructor.
        char character;
        //  A link to the arena.
        Arena* arena;
        //  A base constructor for subclass use.
        Entity(int x, int y, Arena* arena) {
            if (dynamic_cast<Block*>(this) == nullptr) {
                this->id = this->arena->id_incr++;
                this->arena->entityIndex.insert({this->id, this});
            }
            this->x = x;
            this->y = y;
            this->arena = arena;
        }
        //  This method moves the entity:
        //      1.  The method checks if the move is possible. If returned false then the move is not allowed.
        //      2.  The method write x, y if moved.
        //      3.  The method calls arena->move().
        //      4.  The method does all logic needed when moving. e.g. decrease HP, destruct self, etc.
        //      
        //  move() expects x, y to be a valid pixel.
        //  Recommended implementation:
        //  bool move(int x, int y) override {
        //      if (dynamic_cast<T>(arena.pixel[x][y]) != nullptr) { ... }
        //  }
        //  
        virtual bool move(int x, int y) { throw "Entity does not support move."; }
        //  Virtual method for runtime Polymorphism.
        virtual ~Entity() {
            //  Deassign id index.
            if (dynamic_cast<Block*>(this) == nullptr) {
                this->arena->entityIndex.erase(this->id);
            }
        }
    };

    //  Blocks are fixed scene entities that:
    //      Does not move.
    //      Does nothing except blocks entities when moved to.
    class Block: public Entity {
        public:
        Block(int x, int y, Arena* arena): Entity(x, y, arena) {}
        virtual ~Block() {}
        //  Block does not move: the move() is never overrided.
    };

    //  Mob is, well, mob. It moves (likely towards player using A* search), sometimes deals touch damage to the player, and maybe shots bullets.
    class Mob: public Entity {
        public:
        //  All mobs have hp. Bullets are destroyed once collide so hp is not necessary.
        int hp;
        int damage;
        int killScore;
        Mob(int x, int y, Arena* arena, int hp, int damage, int killScore): Entity(x, y, arena) {
            this->hp = hp;
            this->damage = damage;
            this->killScore = killScore;
        }
        //  Mobs may take damage.
        void takeDamage(int damage) {
            this->hp -= damage;
            //  if mob died, clear it and reward score.
            if (hp <= 0) {
                this->arena->game->score += killScore;
                this->arena->remove(this->x, this->y);
            }
        }
        //  The general mob move function.
        bool move(int x, int y) override {
            if (dynamic_cast<Player*>(this->arena->pixel[x][y]) != nullptr) {
                dynamic_cast<Player*>(this->arena->pixel[x][y])->takeDamage(this->damage);
                return false;
            }
            if (dynamic_cast<PlayerBullet*>(this->arena->pixel[x][y]) != nullptr) {
                //  We must store the damage because:
                //      If we move first, the damage data is lost so must be fetched and stored in advance.
                //      If we take damage first, the Mob might be deleted before it deletes the bullet.
                int takeDamage = dynamic_cast<PlayerBullet*>(this->arena->pixel[x][y])->damage;
                this->arena->move(this->x, this->y, x, y);
                this->takeDamage(takeDamage);
            }
            if (dynamic_cast<Air*>(this->arena->pixel[x][y]) != nullptr) {
                this->arena->move(this->x, this->y, x, y);
                return true;
            }
            return false;
        }
        virtual ~Mob() {}
    };

    //  Like the Mob class, the PlayerBullet is also a well-implemented class. But for scalability be aware that some subclass might be implemented in the future...
    class PlayerBullet: public Entity {
        public:
        int damage;
        //  For player bullet direction, 0 = up, 1 = right, 2 = down, 3 = right (clockwise directions)
        //  Notice that since the entity itself is not responsible for searching paths this property is used by external event.
        int direction;
        PlayerBullet(int x, int y, Arena* arena, int direction, int damage): Entity(x, y, arena) {
            this->direction = direction;
            this->damage = damage;
            this->character = '.';
        }
        //  Move (or fly through, precisely)
        bool move(int x, int y) override {
            if (dynamic_cast<Wall*>(this->arena->pixel[x][y]) != nullptr ||
                dynamic_cast<PlayerBullet*>(this->arena->pixel[x][y]) != nullptr ||
                dynamic_cast<Player*>(this->arena->pixel[x][y]) != nullptr) {
                    this->arena->remove(this->x, this->y);
                    return false;
                }
            if (dynamic_cast<Mob*>(this->arena->pixel[x][y]) != nullptr) {
                dynamic_cast<Mob*>(this->arena->pixel[x][y])->takeDamage(this->damage);
                this->arena->replace(this->x, this->y, new Air(this->x, this->y, this->arena));
                return false;
            }
            if (dynamic_cast<Air*>(this->arena->pixel[x][y]) != nullptr) {
                this->arena->move(this->x, this->y, x, y);
                return true;
            }
            return false;
        }
    };

    //  -------------------- Abstract class above --------------------
    //  -------------------- Implementation below --------------------

    //  Wall is the perfect example of minimal entity implementation.
    class Wall: public Block {
        public:
        //  Note: Every entity need to set its character.
        Wall(int x, int y, Arena* arena): Block(x, y, arena) {
            this->character = 'X';
        }
    };

    //  Air is basically the same with air...
    class Air: public Block {
        public:
        Air(int x, int y, Arena* arena): Block(x, y, arena) {
            this->character = ' ';
        }
    };


    //  Player class.
    class Player : public Entity {
        public:
        int hp;
        Player(int x, int y, Arena* arena, int hp): Entity(x, y, arena) {
            this->hp = hp;
            this->character = '@';
        }
        //  Take damage.
        void takeDamage(int damage) {
            this->hp -= damage;
            //  If hp <= 0, throw player died exception.
            if (this->hp <= 0) throw 0;
        }
        //  Move.
        bool move(int x, int y) override {
            if (dynamic_cast<Wall*>(this->arena->pixel[x][y]) != nullptr ||
                dynamic_cast<Mob*>(this->arena->pixel[x][y]) != nullptr) return false;
            if (dynamic_cast<Air*>(this->arena->pixel[x][y]) != nullptr ||
                dynamic_cast<PlayerBullet*>(this->arena->pixel[x][y]) != nullptr) {
                this->arena->move(this->x, this->y, x, y);
                return true;
            }
        }
    };


    //  This is just a random name for the basic mob. I'm bad at naming things...
    class Zombie: public Mob {
        public:
        Zombie(int x, int y, Arena* arena): Mob(x, y, arena, hp=1, damage=1, killScore=1) {
            this->character = 'o';
        }
    };







    //  The abstract EventHandler.
    //  Eventhandlers are where your actual code lives. A EventHandler can be fired to exeucte the event.
    //  Events can have subevents. When event is fired, all its recursive subevents are fired in a DFS pattern.
    class EventHandler {
        public:
        //  A link to the game.
        Game* game;
        //  Base constructor.
        EventHandler(Game* game) {
            this->game = game;
        } 
        //  subEvents: Some event might trigger other events after its execution.
        //      This allows event tree triggering.
        //      Subevents are deleted cascadely. You just need to new them in the constructor.
        vector<EventHandler*> subevents;
        //  The event code.
        virtual void execute() {};
        //  The fire method fires the event. It should be the only method called outside, also not overriden.
        void fire() {
            this->execute();
            for (EventHandler* subevent: this->subevents) subevent->fire();
        }
        //  The virtual destructor helps you automatically destruct all subevents.
        //  It's why they're called subevents and other than this exception you're responsible for deleting everything you new.
        virtual ~EventHandler() {
            for (EventHandler* subevent: this->subevents) delete subevent;
        }
    };


    //  The event fired when calling game.run(). The root event.
    class RunEventHandler: public EventHandler {
        public:
        InitializeEventHandler* initializeEventHandler;
        TickEventHandler* tickEventHandler;
        RunEventHandler(Game* game): EventHandler(game) {
            initializeEventHandler = new InitializeEventHandler(this->game);
            tickEventHandler = new TickEventHandler(this->game);
        }
        void execute() override {
            //  Initialize the game.
            initializeEventHandler->fire();
            //  Run the game.
            //  The game will end with throw endType so there is no need of condition testing.
            while (true) {
                auto now = chrono::system_clock::now();
                tickEventHandler->fire();
                //  Time interval between ticks.
                this_thread::sleep_until(now + chrono::milliseconds(20));
            }
        }
    };

    //  The game initializing EventHandler. Called after Game() constructor, before TickEventHandler.
    class InitializeEventHandler: public EventHandler {
        public:
        InitializeEventHandler(Game* game): EventHandler(game) {
            //  Arena Initialization
            this->game->arena = new Arena(this->game);
            //  This part depends on the stage selected
            //  It currently just initialize the stage with edge walls
            for (int i = 0; i < 102; i++) {
                this->game->arena->replace(0, i, new Wall(0, i, this->game->arena));
                this->game->arena->replace(31, i, new Wall(31, i, this->game->arena));
            }
            for (int j = 1; j < 31; j++) {
                this->game->arena->replace(j, 0, new Wall(j, 0, this->game->arena));
                this->game->arena->replace(j, 101, new Wall(j, 101, this->game->arena));
            }
            //  Player initialization
            //  SINCE PLAYER IS THE FIRST NON_BLOCK ENTITY GENERATED IT ALWAYS HAVE ID 0
            this->game->arena->replace(15, 50, new Player(15, 50, this->game->arena, 5));
        }
    };

    //  The tick event. Called every tick.
    //  Add your event as subevent. The root event itself shouldn't contain any execution.
    class TickEventHandler: public EventHandler {
        public:
        TickEventHandler(Game* game): EventHandler(game) {
            //  Add your new subevent events here.
            this->subevents = {
                new PlayerMoveEventHandler(this->game),
                new PlayerShootEventHandler(this->game),
                new MobGenerateEventHandler(this->game),
                new EntityMoveEventHandler(this->game),
                new ScreenRefreshEventHandler(this->game)
            };
        }
    };
    
    class PlayerMoveEventHandler: public EventHandler {
        public:
        PlayerMoveEventHandler(Game* game): EventHandler(game) {

        }
        void execute() override {

        }
    };
    class PlayerShootEventHandler: public EventHandler {
        public:
        PlayerShootEventHandler(Game* game): EventHandler(game) {

        }
        void execute() override {
            
        }
    };
    class MobGenerateEventHandler: public EventHandler {
        public:
        MobGenerateEventHandler(Game* game): EventHandler(game) {

        }
        void execute() override {
            
        }
    };
    class EntityMoveEventHandler: public EventHandler {
        public:
        EntityMoveEventHandler(Game* game): EventHandler(game) {

        }
        void execute() override {
            
        }
    };
    class ScreenRefreshEventHandler: public EventHandler {
        public:
        ScreenRefreshEventHandler(Game* game): EventHandler(game) {

        }
        void execute() override {
            
        }
    };

}



