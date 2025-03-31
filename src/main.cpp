#include <iostream>

#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/captured_mouse.hpp>
 
int main(void) {
    const int minWidth = 102, minHeight = 30; // TODO: replace with defined constants in core
    ftxui::Dimensions terminalDim = ftxui::Dimension::Full();

    if (terminalDim.dimx < minWidth || terminalDim.dimy < minHeight) {
        auto document = ftxui::hbox({
                ftxui::text("ERROR") | ftxui::bold | ftxui::blink | ftxui::color(ftxui::Color::Red),
                ftxui::separator(),
                ftxui::text("Your terminal is too small. Minimum requirement: 102x30.") | ftxui::xflex,
        }) | ftxui::borderRounded;
        auto screen = ftxui::Screen::Create(
            ftxui::Dimension::Full(),
            ftxui::Dimension::Fit(document)
        );
        ftxui::Render(screen, document);
        screen.Print();

        return 1;
    }

    return 0;
}