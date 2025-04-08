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

## Build Instructions

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

> - `make run` is a shortcut for `make all` and `./main`.
> - Add platform suffix to the target if you are not building on Linux. For example, use `make all_macos` or `make run_macos` for macOS. The supported suffixes are `_macos` and `_windows`.
> - The executable on Windows is `main.exe`.
      
## Coding Requirements

The requirements for this project have been met:

- ✅ **Random event generation** - mob generation depends on random numbers.
- ✅ **Data structures** - the game uses STL containers such as `std::vector`, `std::unordered_map`.
- ✅ **Dynamic memory management** - certain objects are created on runtime using `new` and destructed using `delete`.
- ✅ **File I/O** - the game supports loading a custom map from a file.
- ✅ **Program codes in multiple files** - the project separates the components into multiple files and multiple namespaces.
- ✅ **Multiple difficulty levels** - there are different difficulty levels to choose from.