#include "game_level_ui.hpp"

void gameLvl_configureGameOptions(core::GameOptions* options) {
    gameLvl_gameOptions = options;
    gameLvl_gameOptionsConfigured = true;
}

void gameLvl_mainGameLoop() {
    _game = new core::Game(gameLvl_gameOptions);
    gameLvl_score = _game->Run();
    delete _game;
    gameLvl_gameOptionsConfigured = false;
    gameLvl_gameOptions = nullptr;
}