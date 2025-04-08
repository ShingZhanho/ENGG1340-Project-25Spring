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
//  Checks if the terminal is large enough to run the game.
//  Displays an error and exits with code 1 if the terminal is too small.
void checkTerminalSize();
//  Gets the chosen menu option from the user.
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
    if (ui::ACTUAL_TERMINAL_WIDTH >= ui::MIN_TERMINAL_WIDTH 
     && ui::ACTUAL_TERMINAL_HEIGHT >= ui::MIN_TERMINAL_HEIGHT) return;

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

    const std::vector<std::string> menuOptions = {
        "Start Game",
        "How to Play",
        "About Us",
        "Exit"
    };
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
    ftxui::MenuOption options = ftxui::MenuOption::Vertical();
    options.entries_option.transform = [] (ftxui::EntryState entryState) {
        entryState.label = (entryState.active ? " > " : " | ") + entryState.label;
        ftxui::Element e = ftxui::text(entryState.label);
        if (entryState.active) {
            e |= ftxui::bold;
            e |= ftxui::color(ftxui::Color::Black);
            e |= (entryState.index == 3) ? ftxui::bgcolor(ftxui::Color::Red) : ftxui::bgcolor(ftxui::Color::Green1);
        } 
        if (entryState.focused) {
            e |= (entryState.index == 3) ? ftxui::color(ftxui::Color::Red) : ftxui::color(ftxui::Color::Green1);
        }
        return e;
    };
    auto menu = ftxui::Menu(menuOptions, &selected, options);

    auto renderer = ftxui::Renderer(menu, [&] {
        return ftxui::vbox({
            ftxui::text("\n\n"),
            // -- Game Title ASCII Art --
            ftxui::vbox({
                ftxui::text("  ____  _   _  ___   ___ _____ _ ")    | ftxui::color(ftxui::Color::Green1)    | ftxui::blink,
                ftxui::text(" / ___|| | | |/ _ \\ / _ \\_   _| |")  | ftxui::color(ftxui::Color::Yellow1)   | ftxui::blink,
                ftxui::text(" \\___ \\| |_| | | | | | | || | | |")  | ftxui::color(ftxui::Color::Orange1)   | ftxui::blink,
                ftxui::text("  ___) |  _  | |_| | |_| || | |_|")    | ftxui::color(ftxui::Color::Red)       | ftxui::blink,
                ftxui::text(" |____/|_| |_|\\___/ \\___/ |_| (_)")  | ftxui::color(ftxui::Color::Purple)    | ftxui::blink,
            }) | ftxui::hcenter,
            ftxui::text("\n\n"),
            ftxui::text("A game by ENGG1340 Group 1 (c) 2025") | ftxui::hcenter,
            ftxui::text("\n\n"),

            ftxui::separatorDouble(),

            // -- Menu Title --
            ftxui::text("Main Menu") | ftxui::center | ftxui::bold,

            ftxui::separatorDouble(),

            // -- Menu Options --
            ftxui::text("\n\n"),
            ftxui::vbox({
                ftxui::hbox({
                    ftxui::filler() | ftxui::flex_grow,
                    ftxui::text("Please select an option, and press Enter:\n") | ftxui::hcenter | ftxui::underlined,
                    ftxui::filler() | ftxui::flex_grow
                }),
                ftxui::hbox({
                    ftxui::filler(),
                    ftxui::hbox({
                        ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5),
                        menu->Render() | ftxui::center,
                        ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5)
                    }) | ftxui::border,
                    ftxui::filler()
                }),
                ftxui::filler() | ftxui::flex_grow,
            }) | ftxui::flex_grow,

            ftxui::separator(),

            // -- Menu Description --
            ftxui::vbox({
                ftxui::text("DESCRIPTION:") | ftxui::bold,
                ftxui::hbox({
                    ftxui::text(optionDescriptions.at(selected)),
                    ftxui::text("  (Press Enter to continue.)"),
                })
            }) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 2),
        }) | ftxui::borderDouble | ftxui::flex_grow;
    }) | ftxui::CatchEvent([&] (ftxui::Event event) {
        if (event == ftxui::Event::Return) {
            ui::appScreen.ExitLoopClosure()();
            return true;
        }
        return false;
    })  | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, ui::MIN_TERMINAL_WIDTH) 
        | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, ui::MIN_TERMINAL_HEIGHT)
        | ftxui::center;

    ui::appScreen.Loop(renderer);

    option = selected;
}