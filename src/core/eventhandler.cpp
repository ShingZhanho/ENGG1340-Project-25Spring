//  core components
#include <core/eventhandler.hpp>

namespace core {

    //  BEGIN: EventHandler

    EventHandler::EventHandler(Game* game) {
        this->game = game;
    }

    EventHandler::~EventHandler() {
        for (auto subevent : this->subevents) delete subevent;
    }

    Game* EventHandler::GetGame() {
        return this->game;
    }

    void EventHandler::Fire() {
        this->Execute();
        for (auto subevent : this->subevents) subevent->Fire();
    }

    //  END: EventHandler

}