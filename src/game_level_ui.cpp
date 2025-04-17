#include "game_level_ui.hpp"
#include <util/log.hpp>

void gameLvl_configureGameOptions(core::GameOptions* options) {
    util::WriteToLog("Configuring game options...", "gameLvl_configureGameOptions()");
    gameLvl_gameOptions = options;
}

void gameLvl_mainGameLoop() {
    util::WriteToLog("gameLvl_mainGameLoop() called. Creating core::Game instance...", "gameLvl_mainGameLoop()");
    _game = new core::Game(gameLvl_gameOptions);
    util::WriteToLog("Executing core::Game::Run()...", "gameLvl_mainGameLoop()");
    gameLvl_score = _game->Run();
    delete _game;
    gameLvl_gameOptions = nullptr;
}