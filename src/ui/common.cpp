#include <ui/common.hpp>

#include <ftxui/component/screen_interactive.hpp>

namespace ui {

    const int MIN_TERMINAL_WIDTH = ARENA_WIDTH + 5;
    const int MIN_TERMINAL_HEIGHT = ARENA_HEIGHT + 7;

    ftxui::ScreenInteractive appScreen = ftxui::ScreenInteractive::Fullscreen();

    const int ACTUAL_TERMINAL_WIDTH = ftxui::Dimension::Full().dimx;
    const int ACTUAL_TERMINAL_HEIGHT = ftxui::Dimension::Full().dimy;

    GameUIRenderer* publicGameUIRenderer = nullptr;

}