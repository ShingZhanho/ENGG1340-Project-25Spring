#include <core/game.hpp>

#include <util/log.hpp>

namespace core {

    //  -- Game class ---------------------------------------------

    Game::Game(GameOptions* options) : options(options) { }

    Game::~Game() {
        util::WriteToLog("Deleting game...", "Game::~Game()");
        while (!terminated) {
            util::WriteToLog("Waiting for game to fully terminate...", "Game::~Game()");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        delete runEventHandler;
        if (arenaIsDynamicallyCreated) delete arena;
        util::WriteToLog("Game deleted successfully.", "Game::~Game()");
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

    bool Game::IsInitialised() const {
        return arenaInitialised;
    }

    void Game::SetInitialisationComplete() {
        util::WriteToLog("Game initialisation complete.", "Game::SetInitialisationComplete()");
        arenaInitialised = true;
    }

    bool Game::IsTerminated() const {
        return terminated;
    }

    void Game::SetTerminated() {
        util::WriteToLog("Game fully terminated.", "Game::SetTerminated()");
        terminated = true;
    }

    void Game::ChangeScore(int delta) {
        std::lock_guard<std::mutex> lock(gameMutex);
        score += delta;
    }

    int Game::GetScore() {
        std::lock_guard<std::mutex> lock(gameMutex);
        return score;
    }

    void Game::InitialiseArena() {
        util::WriteToLog("Checking Arena initialisation status...", "Game::InitialiseArena()");
        if (!arenaInitialised) {
            util::WriteToLog("Arena not initialised. Initialising...", "Game::InitialiseArena()");
            if (GetOptions()->GameArena != nullptr) {
                util::WriteToLog("Using provided arena.", "Game::InitialiseArena()");
                arena = GetOptions()->GameArena;
                arena->SetGame(this);
            } else {
                util::WriteToLog("Using default arena.", "Game::InitialiseArena()");
                arena = new Arena();
                arena->SetGame(this);
                arenaIsDynamicallyCreated = true;
            }
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