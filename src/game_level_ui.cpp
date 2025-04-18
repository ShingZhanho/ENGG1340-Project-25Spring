#include "game_level_ui.hpp"
#include <util/log.hpp>
#include <ui/common.hpp>

void gameLvl_configureGameOptions(core::GameOptions* options) {
    util::WriteToLog("Configuring game options...", "gameLvl_configureGameOptions()");
    gameLvl_gameOptions = options;
}

void gameLvl_mainGameLoop() {
    util::WriteToLog("gameLvl_mainGameLoop() called. Creating core::Game instance...", "gameLvl_mainGameLoop()");
    _game = new core::Game(gameLvl_gameOptions);
    ui::publicGameUIRenderer = new ui::GameUIRenderer(_game);
    util::WriteToLog("Executing core::Game::Run()...", "gameLvl_mainGameLoop()");
    gameLvl_score = _game->Run();
    util::WriteToLog("core::Game::Run() completed. Cleaning up...", "gameLvl_mainGameLoop()");
    delete _game;
    delete ui::publicGameUIRenderer;
    ui::publicGameUIRenderer = nullptr;
    gameLvl_gameOptions = nullptr;
    if (gameLvl_customMode) {
        delete gameLvl_gameOptions;
        gameLvl_gameOptions = nullptr;
    }
}