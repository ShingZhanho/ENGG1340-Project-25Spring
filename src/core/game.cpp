#include <core/game.hpp>

namespace core {

    //  -- Game class ---------------------------------------------

    Game::Game(GameOptions options) : options(options) { }

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

        return -1;
    }

    void Game::ChangeScore(int delta) {
        score += delta;
    }

    int Game::GetScore() const {
        return score;
    }

    void Game::InitialiseArena() {
        if (!arenaInitialised) {
            arena = GetOptions().GameArena != nullptr ? GetOptions().GameArena : new Arena();
            arenaInitialised = true;
        }
    }

    Arena* Game::GetArena() const {
        if (!arenaInitialised) return nullptr;
        return arena;
    }

    GameOptions Game::GetOptions() const {
        return options;
    }

    //  -- built-in GameOptions structs -----------------------------------

    namespace DefaultGameOptions {

        const GameOptions EASY = GameOptions{
            .PlayerHp = 100,
            .GameArena = nullptr
        };

        const GameOptions MEDIUM = GameOptions{
            .PlayerHp = 100,
            .GameArena = nullptr
        };

        const GameOptions HARD = GameOptions{
            .PlayerHp = 100,
            .GameArena = nullptr
        };

    } // namespace GameOptions

} // namespace core