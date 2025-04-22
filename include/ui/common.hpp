#ifndef UI_COMMON_HPP
#define UI_COMMON_HPP

// The components shared across the entire life of the application

#include <ftxui/component/screen_interactive.hpp>
#include <ui/game_ui_renderer.hpp>

namespace ui {

    extern const int MIN_TERMINAL_WIDTH;
    extern const int MIN_TERMINAL_HEIGHT;

    // The screen object shared across the entire life time.
    extern ftxui::ScreenInteractive appScreen;

    // The actual terminal dimensions, evaluated at runtime.

    extern const int ACTUAL_TERMINAL_WIDTH;
    extern const int ACTUAL_TERMINAL_HEIGHT;

    // The shared game ui renderer object.
    extern GameUIRenderer* publicGameUIRenderer;

} // namespace ui

#endif // UI_COMMON_HPP