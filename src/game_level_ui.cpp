#include "game_level_ui.hpp"
#include <util/log.hpp>

void gameLvl_configureGameOptions(core::GameOptions* options) {
    gameLvl_gameOptions = options;
    gameLvl_gameOptionsConfigured = true;
}

void gameLvl_mainGameLoop() {
    util::WriteToLog("gameLvl_mainGameLoop() called. Creating core::Game instance...", "gameLvl_mainGameLoop()");
    _game = new core::Game(gameLvl_gameOptions);
    util::WriteToLog("Executing core::Game::Run()...", "gameLvl_mainGameLoop()");
    gameLvl_score = _game->Run();
    delete _game;
    gameLvl_gameOptionsConfigured = false;
    gameLvl_gameOptions = nullptr;
}