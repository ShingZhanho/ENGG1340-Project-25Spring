//  core components
#include <core/eventhandler.hpp>
#include <core/entity.hpp>

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

    //  BEGIN: InitialiseEventHandler

    InitialiseEventHandler::InitialiseEventHandler(Game* game) : EventHandler(game) {
        //  TODO: implement this constructor after Game is implemented
    }

    //  END: InitialiseEventHandler

    //  BEGIN: PlayerMoveEventHandler

    PlayerMoveEventHandler::PlayerMoveEventHandler(Game* game) : EventHandler(game) {}

    // FIXME: Execute() of EventHandler should not be inherited from EventHandler.
    //        as we cannot pass arguments to it.
    //        Since Execute() is only called internally, it can be private and declared separately for each EventHandler.
    // FIXME: The following function, and all other Execute() functions may need refactoring.
    // void PlayerMoveEventHandler::Execute(const ftxui::Event& event){
    //     //  Get player entity
    //     Player* player = dynamic_cast<Player*>(game->arena->entityIndex[0]);
    //     if (!player) return;

    //     //  Get movement
    //     if (event == ftxui::Event::Character('w')) {
    //         player->move(player->x-1, player->y);
    //     } else if (event == ftxui::Event::Character('s')) {
    //         player->move(player->x+1, player->y);
    //     } else if (event == ftxui::Event::Character('a')) {
    //         player->move(player->x, player->y-1);
    //     } else if (event == ftxui::Event::Character('d')) {
    //         player->move(player->x, player->y+1);
    //     }
    // }

    // END: PlayerMoveEventHandler
}
