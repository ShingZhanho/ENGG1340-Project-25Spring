//  ASCII-only is recommended for compatibility.
//  All comments begin with // then 2 spaces. In total that's 4 character, the length of a typical tab.
//  YOU MUST COMMENT YOUR CODE.


//  Some general C++ tips:
//  1.  Use '' for char. "" is char*.
//  2.  Remember to delete your newed objects. A common practice is do cascadely delete all newed childs in the destructor.
//      Write delete right after you write new to prevent memory leak.


//  Basic data structure
#include <vector>

#include "game.hpp"

using namespace std;

// The main object representing the whole round.
class Game {
    public:
    //  Variable declaration.
    //  The score. Initial score is 0.
    int score;
    //  The arena. refer to class definition for detail.
    Arena* arena;
    //  The screen refer to class definition for detail.
    Screen* screen;

    //  The entry point. A game is started by new Game().run()  
    int run() {
        //  Initialize declared variables.
        this->score = 0;
        this->arena = new Arena();
        this->screen = new Screen();
        //  The main loop.
        bool gameOver = false;
        while (!gameOver) {
            runTick();
        }
        //  Game over.
        return score;
    }
    //  Run a tick. The main loop.
    int runTick() {

    }
    //  Cascade destruct.
    ~Game() {
        delete arena;
        delete screen;
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
//  X 0 0XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//      1X -                                                                                                  X
//      2X|+---------------------------||--------------------------------------||----------------------------|X
//      3X |         30 wall                   40 wall + Mob-spawning area                 30 wall
//      4X |
//      5X | 10 wall
//      6X |
//      7X |
//      8X |
//      9X |
//    1 0X -
//      1M -
//      2M |
//      3M |
//      4M |
//      5M | 10 Wall + Mob-spawning area
//      6M |
//      7M |
//      8M |
//      9M |
//    2 0M -
//      1X -
//      2X |
//      3X |
//      4X |
//      5X | 10 wall
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
//      M -> Edge wall. But spawns mob at the inner empty block.
//
//      @ -> Player.
//      z -> Mob. Zombies.
//      Z -> Mob. Elite Zombies.
//      O -> Mob. Ranger.
//
//      . -> Player bullet.
//      o -> Ranger bullet.
//
class Arena {
    public:
    //  'Pixel' is the name for the place, block... In this project, we're calling it pixel. We need to unify this name.
    Entity* pixel[32][102];
    //  Construct the arena. Nothing is placed inside except air.
    Arena() {
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                pixel[i][j] = new Air(i, j);
            }
        }
    }
    //  Destructs all alive entities.
    ~Arena() {
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                delete pixel[i][j];
            }
        }
    }
};


//  The rendered output.
//  This class looks similar to Arena, but Arena is used for internal logic and this is for the output.
//  Only one screen instance is needed for the whole game - because we need to refresh all pixel[32][102] for every frame anyway.
class Screen {
    public:
    //  The pixels.
    char pixel[32][102];
    //  Refresh a new frame.
    void refresh(Arena* arena) {
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 32; j++) {
                this->pixel[i][j] = arena->pixel[i][j]->character;
            }
        }
    }
};


//  The parent class of all entities: wall, player, mob, bullets, air, etc. Anything in the arena counts, even air.
class Entity {
    public:
    //  Basic fields. These fields should be filled in subclass constructor.

    //  The position (x, y).
    //  Any entity must occupy exact 1 position in the arena.
    int x;
    int y;
    //  The output character. This is a class member, same for all instance.
    char character;
    //  This method moves the entity:
    //      1.  The method checks if the move is possible. If returned false then the move is not allowed.
    //      2.  The method write x, y if moved.
    //      3.  The method assign new Air() to the starting pixel, delete Entity() at the destination pixel, then assign itself to it.
    //      4.  The method does all logic needed when moving. e.g. decrease HP, destruct self, etc.
    //      
    //  Recommended implementation:
    //  bool move(int x, int y, Arena* arena) override {
    //      switch (arena->pixel[x][y]) { ... };
    //  }
    //  
    bool virtual move(int x, int y, Arena* arena) {};
};

//  The Air is a perfect example for minimal entity implementation.
class Air: public Entity {
    public:
    Air(int x, int y) {
        this->x = x;
        this->y = y;
        this->character = ' ';
    }
    bool move(int x, int y, Arena* arena) {
        return false;
    }
};

class Wall: public Entity {
    public:
    Wall(int x, int y) {
        this->x = x;
        this->y = y;
        this->character = 'X';
    }
    bool move(int x, int y, Arena* arena) {
        return false;
    }
};

class MobWall: public Entity {
    public:
    MobWall(int x, int y) {
        this->x = x;
        this->y = y;
        this->character = 'M';
    }
    bool move(int x, int y, Arena* arena) {
        return false;
    }
};