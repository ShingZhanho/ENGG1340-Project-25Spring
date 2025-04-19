# SHOOT!

<p align="center">
   <img src="docs/game_logo.gif" alt="Logo of the game, blinking."/>
   <br/>
   <span>Made by ENGG1340 Group 1. (2024/25 Spring Semester)</span>
</p>

## Team Members (alphabetically)

- CHAN, Ho Nam (3036235352) [@ConanChan0113](https://gihub.com/ConanChan0113)
- LEUNG, Ming Ngai (3036393221) [@ThomasL956](https://github.com/ThomasL956)
- LIU, Jia Lin (3036391493) [@Kylineason-Liu](https://github.com/Kylineason-Liu)
- SHING, Zhan Ho Jacob (3036228892) [@ShingZhanho](https://github.com/ShingZhanho)
- SONG, Sizhe （3036457247）[@TIP-SoNg](https://github.com/TIP-SoNg)
- XIN, Yucheng (3036289755) [@Scevenate](https://github.com/Scevenate)  

##  

Welcome to _SHOOT!_, a high-octane, adrenaline-fueled shooting game where quick reflexes, sharp aim, and tactical thinking  
are your only keys to survival! Step into a deadly arena where monstrous foes emerge relentlessly from every direction, and  
your mission is simple: **shoot to survive, survive to dominate**.  

## The Battlefield Awaits

You are the lone warrior in a hostile arena, armed with nothing but your wits and a powerful gun. The arena is a grid-based  
battleground where every pixel could mean life or death—walkable terrain, deadly monsters, or you, the last line of defense.  
Monsters spawn unpredictably from the four boundaries, forcing you to stay alert, move strategically, and fire with precision.

## Rules of Survival

_SHOOT!_ is straightforward but brutally intense:  
1. **Eliminate or be eliminated**: Monsters will keep coming and your job is to gun them down before they overrun you.
2. **One mistake can be fatal**: Some monsters are faster, tougher, or deadlier than others. Learn their behaviors or face a quick defeat.
3. **Score to win**: Every kill adds to your score. Push your limits, beat your high score, and prove your dominance.
4. **No Escape**: The arena is locked! Fight until the last monster falls… or until you do.

## Features That Will Keep You Hooked

1. **Monster mayhem-no two enemies are the same**: Randomized spawn is based on generating random numbers in every round. Every monster
   in the wild arena will or will not be identical. You may never face the same wave twice. Some creatures charge recklessly, while others
   lurk and strike when you least expect it. Adapt or perish—the arena shows no mercy.  
2. **Fully customizable arenas**: Why stick to the default battlefield when you can design your own? _SHOOT!_ is equipped with fileIO which  
   allows you to import custom map files, and set up your own initial player position. The freedom is in your hands.
3. **Difficulty levels for every player**: Whether you are a novice seeking a gentle introduction to the chaos, a veteran who survives relent
   less waves and is equipped with precise skills, or a elite shooter, there is always a difficulty level just for you. In each level, the
   monster spawn rates, their speed and their amount are always changing. Get ready to push your limits and conquer the arena!
4. **Dynamic combat–fast, fluid, and ferocious**: Bullets fly, monsters swarm, and every second counts. The game’s smooth mechanics ensure
   that your skill—not luck—determines victory.
5. **Glory of the achievers**: After each round, your score is automatically stored into the leaderboard. Compete against yourself or others.
   How long can you last? How many monsters can you slay before the inevitable end? The leaderboard awaits the boldest shooters.





## How to Play

1. Change your working directory to `build`:
   ```bash
   cd build
   ```
2. `make` the target:
   ```bash
   make all
   ```
3. Run the program:
   ```bash
   ./main
   ```
   Please note that:
> * `make run` is a shortcut for `make all` and `./main`.  
> * Add platform suffix to the target if you are not building on Linux. For example, use `make all_macos` or `make run_macos` for macOS. The supported suffixes are `_macos` and `_windows`.  
> * The executable on Windows is `main.exe`.
      
## Coding Requirements

The requirements for this project have been met:

- ✅ **Random event generation** - mob generation depends on random numbers.
- ✅ **Data structures** - the game uses STL containers such as `std::vector`, `std::unordered_map`.
- ✅ **Dynamic memory management** - certain objects are created on runtime using `new` and destructed using `delete`.
- ✅ **File I/O** - the game supports loading a custom map from a file.
- ✅ **Program codes in multiple files** - the project separates the components into multiple files and multiple namespaces.
- ✅ **Multiple difficulty levels** - there are different difficulty levels to choose from.
