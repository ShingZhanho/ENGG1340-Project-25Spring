#include <ui/common.hpp>
#include <ui/game_ui_renderer.hpp>

#include <util/log.hpp>

#include <ftxui/component/component.hpp>

namespace ui {

    GameUIRenderer::GameUIRenderer(core::Game* game) : game(game) { }

    void GameUIRenderer::StartRenderLoop() {
        util::WriteToLog("Starting game UI renderer...", "GameUIRenderer::StartRenderLoop()");

        auto container = ftxui::Container::Vertical({});
        ui::RenderOption entityRenderer;
        for (int y = 0; y < ARENA_HEIGHT; ++y) {
            auto row = ftxui::Container::Horizontal({});
            for (int x = 0; x < ARENA_WIDTH; ++x) {
                entityRenderer = game->GetArena()->GetPixel({x, y})->GetRenderOption();
                row->Add(ftxui::Renderer([&] {
                    return entityRenderer.Render();
                }));
            }
            container->Add(row);
        }
        auto ui = ftxui::Renderer(container, [&] {
            return ftxui::vbox({
                ftxui::text("Game Screen") | ftxui::center | ftxui::bold,
                ftxui::separator(),
                container->Render()
            });
        }) | ftxui::CatchEvent([&] (ftxui::Event event) {
            if (event == ftxui::Event::Escape) {
                game->Terminate();
                appScreen.ExitLoopClosure()();
                return true;
            }
            return false;
        });
        ui::appScreen.Loop(ui);
    }

    void GameUIRenderer::RefreshUI() {
        util::WriteToLog("Refresh of game UI requested...", "GameUIRenderer::RefreshUI()");
        ui::appScreen.Post(ftxui::Event::Custom);
    }

}