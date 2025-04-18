#include <core/game.hpp>

#include <util/log.hpp>

namespace core {

    //  -- Game class ---------------------------------------------

    Game::Game(GameOptions* options) : options(options) { }

    Game::~Game() {
        delete runEventHandler;
        //  The arena is constructed in the event handler,
        //  so it should be deleted in the event handler.
    }

    int Game::Run() {
        try {
            util::WriteToLog("Starting game...", "Game::Run()");
            running = true;
            runEventHandler = new RunEventHandler(this);
            util::WriteToLog("Triggering run event handler...", "Game::Run()");
            runEventHandler->Fire();
        } catch (int endType) {
            return score;
        }

        return -1;
    }

    void Game::Terminate() {
        util::WriteToLog("Game termination requested.", "Game::Terminate()");
        running = false;
    }

    bool Game::IsRunning() const {
        return running;
    }

    void Game::ChangeScore(int delta) {
        score += delta;
    }

    int Game::GetScore() const {
        return score;
    }

    void Game::InitialiseArena() {
        util::WriteToLog("Checking Arena initialisation status...", "Game::InitialiseArena()");
        if (!arenaInitialised) {
            util::WriteToLog("Arena not initialised. Initialising...", "Game::InitialiseArena()");
            arena = GetOptions()->GameArena != nullptr ? GetOptions()->GameArena : new Arena();
            arenaInitialised = true;
            return;
        }
        util::WriteToLog("Arena already initialised.", "Game::InitialiseArena()");
    }

    Arena* Game::GetArena() const {
        if (!arenaInitialised) return nullptr;
        return arena;
    }

    GameOptions* Game::GetOptions() const {
        return options;
    }

} // namespace core