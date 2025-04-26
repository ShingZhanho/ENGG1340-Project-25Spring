// game_level_ui defines functions for the game level UI,
// the code is not part of the core or ui namespaces, therefore
// the header file is not placed in the /include directory.
// Other files of the same nature should observe this same practice.

#ifndef _GAME_LEVEL_UI_HPP // Use preceding underscore to indicate a non-namespaced header file
#define _GAME_LEVEL_UI_HPP

// ==== IMPORTANT ====
// All variables/functions that will be used outside this file MUST begin with "gameLvl_"
// All variables/functions that will be used only in this file MUST be defined in the .cpp file, not here.

#include <core/game.hpp>

#include <atomic>

// The game options to be used in the game loop.
static core::GameOptions* gameLvl_gameOptions;
// To configure the game options.
void gameLvl_configureGameOptions(core::GameOptions* options);
// Indicates if the game options are constructed by custom mode.
static std::atomic<bool> gameLvl_customMode = false;
// The main game loop.
void gameLvl_mainGameLoop();
// The score of the game.
static int gameLvl_score = 0;

#endif // _GAME_LEVEL_UI_HPP