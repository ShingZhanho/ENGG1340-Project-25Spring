#ifndef UI_GAME_UI_RENDERER_HPP
#define UI_GAME_UI_RENDERER_HPP

#include <core/game.hpp>
#include <ftxui/component/component.hpp>

namespace ui {

    //  Responsible for rendering the game UI.
    class GameUIRenderer {
        public:
            //  Constructor
            GameUIRenderer(core::Game* game);
            //  Takes over ui::appScreen for rendering the game's UI.
            //  This function must be called in the main thread, NOT from the tick thread.
            void StartRenderLoop();
            //  Refreshes the game UI. This method is thread-safe.
            void RefreshUI();

        private:
            core::Game* game;
            ftxui::Component draw();
    };
    
}

#endif // UI_GAME_UI_RENDERER_HPP