#include <ui/common.hpp>
#include <ui/game_ui_renderer.hpp>

#include <util/log.hpp>

#include <ftxui/component/component.hpp>

#include <vector>
#include <string>

namespace ui {

    GameUIRenderer::GameUIRenderer(core::Game* game) : game(game) { }

    void GameUIRenderer::StartRenderLoop() {
        util::WriteToLog("Starting game UI renderer...", "GameUIRenderer::StartRenderLoop()");
        auto container = ftxui::Container::Vertical({});
        auto ui = ftxui::Renderer(container, [&] {
            std::vector<ftxui::Element> allRows;
            for (int y = 0; y < ARENA_HEIGHT; y++) {
                std::vector<ftxui::Element> rowElements;
                for (int x = 0; x < ARENA_WIDTH; x++) {
                    auto entityRenderer = game->GetArena()->GetPixel({x, y})->GetRenderOption();
                    rowElements.push_back(entityRenderer.Render());
                }
                allRows.push_back(ftxui::hbox(rowElements));
            }
            auto rows = ftxui::vbox(allRows);
            return rows | ftxui::center;
        }) | ftxui::CatchEvent([&] (ftxui::Event event) {
            util::WriteToLog("UI Event captured.", "GameUIRenderer::StartRenderLoop()");
            if (event == ftxui::Event::Escape) { // Exit the game (temporary)
                appScreen.ExitLoopClosure()();
                game->Terminate();
                return true;
            } else if (event == ftxui::Event::Custom) {
                return true;
            } else if (event == ftxui::Event::Character('w')) {
                game->PlayerMoveEventHandlerPtr->SetDirection(core::PlayerMoveEventHandler::Direction::UP);
                game->PlayerMoveEventHandlerPtr->Fire();
                return true;
            } else if (event == ftxui::Event::Character('s')) {
                game->PlayerMoveEventHandlerPtr->SetDirection(core::PlayerMoveEventHandler::Direction::DOWN);
                game->PlayerMoveEventHandlerPtr->Fire();
                return true;
            } else if (event == ftxui::Event::Character('a')) {
                game->PlayerMoveEventHandlerPtr->SetDirection(core::PlayerMoveEventHandler::Direction::LEFT);
                game->PlayerMoveEventHandlerPtr->Fire();
                return true;
            } else if (event == ftxui::Event::Character('d')) {
                game->PlayerMoveEventHandlerPtr->SetDirection(core::PlayerMoveEventHandler::Direction::RIGHT);
                game->PlayerMoveEventHandlerPtr->Fire();
                return true;
            }
            util::WriteToLog("UI redraw not needed. Skipping...", "GameUIRenderer::StartRenderLoop()");
            return false;
        });
        ui::appScreen.Loop(ui);
    }
}