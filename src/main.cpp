// Standard Libraries
#include <iostream>
#include <map>
#include <vector>
#include <string>

// FTXUI
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/captured_mouse.hpp>

// Core Components
#include <ui/common.hpp>
#include <core/arena.hpp>

// Declarations
void checkTerminalSize();
void getMenuOption(int& option);
 
int main(void) {
    // Check if the terminal is large enough
    checkTerminalSize();

    int menuOption = 0;
    while (menuOption != 3) { // 3 = "Exit"
        getMenuOption(menuOption);
    }

    std::cout << "User selected option: " << menuOption << std::endl;

    return 0;
}

void checkTerminalSize() {// Check if the terminal is large enough
    if (ui::ACTUAL_TERMINAL_WIDTH >= ui::MIN_TERMINAL_WIDTH && ui::ACTUAL_TERMINAL_HEIGHT >= ui::MIN_TERMINAL_HEIGHT) return;

    auto document = ftxui::hbox({
            ftxui::vbox({
                ftxui::text("ERROR") | ftxui::bold | ftxui::blink | ftxui::color(ftxui::Color::Red),
                ftxui::text("X") | ftxui::bold | ftxui::blink | ftxui::color(ftxui::Color::Red) | ftxui::hcenter,
            }),
            ftxui::separator(),
            ftxui::vbox({
                ftxui::text(
                    "Your terminal is too small. Minimum requirement: "
                    + std::to_string(ui::MIN_TERMINAL_WIDTH) + " x "
                    + std::to_string(ui::MIN_TERMINAL_HEIGHT) + "."
                ),
                ftxui::text("Please try resizing your terminal window, and then run the program again."),
            })
    }) | ftxui::borderRounded;
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),
        ftxui::Dimension::Fit(document)
    );
    ftxui::Render(screen, document);
    screen.Print();

    exit(1);
    
}

void getMenuOption(int& option) {
    int selected = 0;

    const std::vector<std::string> descriptionStrings = {
        "Starts the game.",
        "Learn how to play the game.",
        "See information about the game and the developers.",
        "Exit the game."
    };
    std::unordered_map<int, std::string> optionDescriptions = {};
    for (size_t i = 0; i < descriptionStrings.size(); i++) {
        optionDescriptions[i] = descriptionStrings[i];
    }
    
    // the following code implements an animated coloured menu
    // the code snippet is adapted from the author of the FTXUI library
    // at https://arthursonzogni.github.io/FTXUI/examples_2component_2menu_entries_animated_8cpp-example.html
    auto ColouredOption = [](ftxui::Color c) {
        ftxui::MenuEntryOption option;
        option.animated_colors.foreground.enabled = true;
        option.animated_colors.background.enabled = true;
        option.animated_colors.background.active = c;
        option.animated_colors.background.inactive = ftxui::Color::Default;
        option.animated_colors.foreground.active = ftxui::Color::Grey0;
        option.animated_colors.foreground.inactive = c;
        return option;
    };

    auto menu = ftxui::Container::Vertical({
        ftxui::MenuEntry("1. Start Game", ColouredOption(ftxui::Color::Green1)),
        ftxui::MenuEntry("2. How to Play", ColouredOption(ftxui::Color::Cyan)),
        ftxui::MenuEntry("3. About Us", ColouredOption(ftxui::Color::Blue1)),
        ftxui::MenuEntry("4. Exit", ColouredOption(ftxui::Color::Red)),
    }, &selected);

    auto renderer = ftxui::Renderer(menu, [&] {
        return ftxui::window(
            ftxui::text("Main Menu") | ftxui::center | ftxui::bold,
            ftxui::vbox({
                ftxui::text("Please select an option, and press Enter:"),
                ftxui::separator(),
                menu->Render() | ftxui::frame | ftxui::center | ftxui::yflex_grow,
                ftxui::separator(),
                ftxui::vbox({
                    ftxui::text("DESCRIPTION:") | ftxui::bold,
                    ftxui::hbox({
                        ftxui::text(optionDescriptions.at(selected)),
                        ftxui::text("  (Press Enter to continue.)"),
                    })
                })
            })
        );
    }) | ftxui::CatchEvent([&] (ftxui::Event event) {
        if (event == ftxui::Event::Return) {
            ui::appScreen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    ui::appScreen.Loop(renderer);

    option = selected;
}