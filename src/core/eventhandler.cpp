//  core components
#include <core/eventhandler.hpp>

namespace core {

    //  BEGIN: EventHandler

    EventHandler::EventHandler(Game* game) {
        game = game;
    }

    EventHandler::~EventHandler() {
        for (auto subevent : subevents) delete subevent;
    }

    Game* EventHandler::GetGame() {
        return game;
    }

    void EventHandler::Fire() {
        Execute();
        for (auto subevent : subevents) subevent->Fire();
    }

    //  END: EventHandler

    //  BEGIN: RunEventHandler

    RunEventHandler::RunEventHandler(Game* game) : EventHandler(game) {
        initialiseEventHandler = new InitialiseEventHandler(game);
        tickEventHandler = new TickEventHandler(game);
    }

    void RunEventHandler::Execute() {
        //  Initialize the game.
        initialiseEventHandler->Fire();
        //  Run the game.
        //  The game will end with throw endType so there is no need of condition testing.
        while (true) {
            auto now = std::chrono::system_clock::now();
            tickEventHandler->Fire();
            //  Time interval between ticks.
            std::this_thread::sleep_until(now + std::chrono::milliseconds(20));
        }
    }

    //  END: RunEventHandler

}