#include "game_level_ui.hpp"
#include <util/log.hpp>
#include <ui/common.hpp>
#include "game_score_ui.hpp"

static core::Game* _game = nullptr;

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
    util::WriteToLog("core::Game::Run() completed. Checking exit reason...", "gameLvl_mainGameLoop()");

    if (_game->GetTerminateReason() == 0) {
        util::WriteToLog("Game terminated due to HP reaching 0.", "gameLvl_mainGameLoop()");
        util::WriteToLog("Game score: " + std::to_string(gameLvl_score), "gameLvl_mainGameLoop()");
        util::WriteToLog("Displaying score UI...", "gameLvl_mainGameLoop()");
        gameScore_displayScore(gameLvl_score);
    } else if (_game->GetTerminateReason() == 1) {
        util::WriteToLog("Game terminated due to player quitting.", "gameLvl_mainGameLoop()");
        util::WriteToLog("Discarding score...", "gameLvl_mainGameLoop()");
    } else {
        util::WriteToLog("Game terminated due to unknown reason.", "gameLvl_mainGameLoop()");
    }

    util::WriteToLog("Deleting core::Game instance...", "gameLvl_mainGameLoop()");
    delete _game;
    _game = nullptr;

    util::WriteToLog("Deleting ui::publicGameUIRenderer instance...", "gameLvl_mainGameLoop()");
    delete ui::publicGameUIRenderer;
    ui::publicGameUIRenderer = nullptr;
    

    if (gameLvl_customMode) {
        util::WriteToLog("Deleting gameLvl_gameOptions instance...", "gameLvl_mainGameLoop()");
        delete gameLvl_gameOptions;
        gameLvl_gameOptions = nullptr;
    }
}