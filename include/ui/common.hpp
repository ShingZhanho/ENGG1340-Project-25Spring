#ifndef UI_COMMON_HPP
#define UI_COMMON_HPP

// The components shared across the entire life of the application

#include <core/arena.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace ui {

    extern const int MIN_TERMINAL_WIDTH;
    extern const int MIN_TERMINAL_HEIGHT;

    // The screen object shared across the entire life time.
    extern ftxui::ScreenInteractive appScreen;

    // The actual terminal dimensions, evaluated at runtime.

    extern const int ACTUAL_TERMINAL_WIDTH;
    extern const int ACTUAL_TERMINAL_HEIGHT;

} // namespace ui

#endif // UI_COMMON_HPP