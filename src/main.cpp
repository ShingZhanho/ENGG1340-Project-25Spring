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
// -- Main Menu Functions -------------------------------------------------------

//  Checks if the terminal is large enough to run the game.
//  Displays an error and exits with code 1 if the terminal is too small.
void checkTerminalSize();
//  Gets the chosen menu option from the user.
void getMenuOption(int& option);

// -- Game Difficulty Menu Functions -------------------------------------------------------

//  Main loop for handling the game difficulty menu UI.
void difficultyMenu();
 
int main(void) {
    // Check if the terminal is large enough
    checkTerminalSize();

    int menuOption = 0;
    while (menuOption != 3) { // 3 = "Exit"
        getMenuOption(menuOption);
        switch (menuOption) {
            case 0: // Start Game
                difficultyMenu();
                break;
            case 1: // How to Play
                // TODO:
                break;
            case 2: // About us
                // TODO:
                break;
            default:
                break;
        }
    }

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

//  -- Main Menu Functions -------------------------------------------------------

void getMenuOption(int& option) {
    int selected = 0;

    const std::vector<std::string> menuOptions = {
        "Start Game",
        "How to Play",
        "About Us",
        "Exit"
    };
    const std::vector<std::string> descriptionStrings = {
        " Starts the game.",
        " Learn how to play the game.",
        " See information about the game and the developers.",
        " Exit the game."
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
                ftxui::text(" DESCRIPTION:") | ftxui::bold,
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

//  -- Game Difficulty Menu Functions -------------------------------------------------------

void difficultyMenu() {
    // -- Game Difficulty Menu (Radio Buttons)
    int selectedDifficulty = 0;
    bool loadCustomGame = false;
    auto radioOption = ftxui::RadioboxOption();
    radioOption.on_change = [&] { loadCustomGame = (selectedDifficulty == 3); };
    radioOption.transform = [&] (ftxui::EntryState state) {
        state.label = (state.state ? " [*] " : " [ ] ") + state.label;
        ftxui::Element e = ftxui::text(state.label);
        ftxui::Color activeFgcolour;
        switch (state.index) {
            case 0: activeFgcolour = ftxui::Color::Green1; break; // Easy
            case 1: activeFgcolour = ftxui::Color::Yellow1; break; // Medium
            case 2: activeFgcolour = ftxui::Color::Red; break; // Hard
            case 3: activeFgcolour = ftxui::Color::Orange1; break; // Custom
        }
        if (state.state) e = e | ftxui::bold | ftxui::color(activeFgcolour);
        if (state.focused) e = e | ftxui::color(activeFgcolour);
        return e;
    };
    const std::vector<std::string> difficultyOptions = {
        "Easy",
        "Medium",
        "Hard",
        "Load a custom game"
    };
    auto difficultyRadioButtons = ftxui::Radiobox(&difficultyOptions, &selectedDifficulty, radioOption);

    // -- Back button
    auto backButton = ftxui::Button(" < Back to Menu ", ui::appScreen.ExitLoopClosure());

    // -- Start button
    auto startButton = ftxui::Button(" Start Game > ", [] {}); // TODO: Implement start game function

    // -- Custom Game Options
    // placeholder
    auto customGameOptionsMenu = ftxui::Container::Horizontal({});
    auto placeholder = ftxui::Renderer(customGameOptionsMenu, [&] {return ftxui::text("Selected: custom game");});

    // -- Wrapper Box
    auto layout = ftxui::Container::Vertical({
        ftxui::Renderer([] {
            return ftxui::vbox({
                ftxui::text("Select a difficulty level:") | ftxui::center | ftxui::bold,
                ftxui::separator(),
            });
        }),
        difficultyRadioButtons | ftxui::borderDouble,
        placeholder | ftxui::Maybe(&loadCustomGame),
        ftxui::Renderer([] {return ftxui::filler();}),
        ftxui::Container::Horizontal({
            backButton, startButton
        }) | ftxui::center
    })  | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, ui::MIN_TERMINAL_WIDTH) 
        | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, ui::MIN_TERMINAL_HEIGHT)
        | ftxui::center;

    // Render the menu
    ui::appScreen.Loop(layout);
}