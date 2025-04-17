// game_level_ui defines functions for the game level UI,
// the code is not part of the core or ui namespaces, therefore
// the header file is not placed in the /include directory.
// Other files of the same nature should observe this same practice.

#ifndef _GAME_LEVEL_UI_HPP // Use preceding underscore to indicate a non-namespaced header file
#define _GAME_LEVEL_UI_HPP

// ==== IMPORTANT ====
// All variables/functions that will be used outside this file MUST begin with "gameLvl_"
// All variables/functions that will be used only in this file MUST begin with "_"

#include <core/game.hpp>

// The game options to be used in the game loop.
static core::GameOptions* gameLvl_gameOptions;
// To check if the game options have been properly configured.
// Do not modify this variable outside this file.
static bool gameLvl_gameOptionsConfigured = false;
// To configure the game options.
void gameLvl_configureGameOptions(core::GameOptions* options);
// The main game loop.
void gameLvl_mainGameLoop();
// The score of the game.
static int gameLvl_score = 0;
// The private Game object.
static core::Game* _game;

#endif // _GAME_LEVEL_UI_HPP