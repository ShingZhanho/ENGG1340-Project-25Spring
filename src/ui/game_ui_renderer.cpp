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
        auto ui = ftxui::Renderer([&] {
            //  Render the game arena
            std::vector<ftxui::Element> allRows;
            for (int y = 0; y < ARENA_HEIGHT; y++) {
                std::vector<ftxui::Element> rowElements;
                for (int x = 0; x < ARENA_WIDTH; x++) {
                    auto entityRenderer = game->GetArena()->GetPixel({x, y})->GetRenderOption();
                    rowElements.push_back(entityRenderer.Render());
                }
                allRows.push_back(ftxui::hbox(rowElements));
            }
            auto rows = ftxui::vbox(allRows)    | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, ARENA_WIDTH) 
                                                | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, ARENA_HEIGHT)
                                                | ftxui::hcenter;

            //  Render other components
            float hp = (float) dynamic_cast<core::Player*>(game->GetArena()->GetPixelById(0))->GetHP() / (float) game->GetOptions()->PlayerHp;
            auto hpColour = hp > 0.5 ? ftxui::Color::Green : (hp > 0.25 ? ftxui::Color::Yellow : ftxui::Color::Red);

            return ftxui::vbox({
                rows,
                ftxui::separator(),
                ftxui::hbox({
                    ftxui::text(" HP: ") | ftxui::bold,
                    ftxui::gaugeLeft(hp)    | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15)
                                            | ftxui::color(hpColour),
                    ftxui::text(" " + std::to_string(dynamic_cast<core::Player*>(game->GetArena()->GetPixelById(0))->GetHP()) + " / " + std::to_string(game->GetOptions()->PlayerHp) + " ")
                        | ftxui::color(hpColour),
                    ftxui::separator(),
                    ftxui::text(" Score: ") | ftxui::bold,
                    ftxui::text(std::to_string(game->GetScore()) + " ") | ftxui::color(ftxui::Color::Cyan),
                    ftxui::separator(),
                    ftxui::text(" Press ESC to abort (score will not be saved).")
                }),
                ftxui::separator(),
                ftxui::text(" INSTRUCTIONS:") | ftxui::bold,
                ftxui::paragraphAlignLeft("  - Move: W/A/S/D"),
                ftxui::paragraphAlignLeft("  - Shoot a bullet: I/J/K/L")
            })  | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, MIN_TERMINAL_WIDTH)
                | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, MIN_TERMINAL_HEIGHT + 1)
                | ftxui::borderRounded;
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
        })  | ftxui::center;
        ui::appScreen.Loop(ui);
    }
}