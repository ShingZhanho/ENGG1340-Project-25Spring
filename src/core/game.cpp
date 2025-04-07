#include <core/game.hpp>

namespace core {

    Game::Game() {

    }

    Game::~Game() {
        delete runEventHandler;
        //  The arena is constructed in the event handler,
        //  so it should be deleted in the event handler.
    }

    int Game::Run() {
        try {
            runEventHandler = new RunEventHandler(this);
            runEventHandler->Fire();
        } catch (int endType) {
            return score;
        }
    }

    void Game::ChangeScore(int delta) {
        score += delta;
    }

    int Game::GetScore() const {
        return score;
    }

    void Game::InitialiseArena() {
        if (!arenaInitialised) {
            arena = new Arena();
            arenaInitialised = true;
        }
    }

    Arena* Game::GetArena() const {
        if (!arenaInitialised) return nullptr;
        return arena;
    }

}