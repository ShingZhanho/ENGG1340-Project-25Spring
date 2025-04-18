// Standard Libraries
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <fstream>

// FTXUI
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/captured_mouse.hpp>

// Core Components
#include <ui/common.hpp>
#include <core/arena.hpp>
#include <core/arena_reader.hpp>
#include <core/entity_type.hpp>

// Misc headers
#include "game_level_ui.hpp"
#include <util/log.hpp>

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
// Wraps a component with a field name and a separator.
// Takes a field name and a component to be wrapped, returns the wrapped component.
// The field name must be less than or equal to 15 characters, otherwise it is truncated and "..." is added.
ftxui::Component wrapComponent(std::string fieldName, ftxui::Component component);

// -- Custom Game Options Functions -------------------------------------------------------

//  Gets the selected mob types from the custom game options.
std::set<core::EntityType> getMobTypes(std::unordered_map<core::EntityType, bool*> mobFlags);
//  Sets the error message and shows the error message.
void setErrorMessage(std::string& errorMessage, std::string msg, bool& showError);
 
int main(void) {
    util::WriteToLog("Process started.", "main()");
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
    util::WriteToLog("Checking terminal size...");
    util::WriteToLog("Required size: " + std::to_string(ui::MIN_TERMINAL_WIDTH) + " x " + std::to_string(ui::MIN_TERMINAL_HEIGHT) + ". Actual size: " + std::to_string(ui::ACTUAL_TERMINAL_WIDTH) + " x " + std::to_string(ui::ACTUAL_TERMINAL_HEIGHT) + ".");

    if (ui::ACTUAL_TERMINAL_WIDTH >= ui::MIN_TERMINAL_WIDTH 
     && ui::ACTUAL_TERMINAL_HEIGHT >= ui::MIN_TERMINAL_HEIGHT) {
        util::WriteToLog("Terminal size check passed.");
        return;
    }

    auto document = ftxui::hbox({
            ftxui::vbox({
                ftxui::text("ERROR") | ftxui::bold | ftxui::blink | ftxui::color(ftxui::Color::Red),
                ftxui::text("X") | ftxui::bold | ftxui::blink | ftxui::color(ftxui::Color::Red) | ftxui::hcenter,
            }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5),
            ftxui::separator(),
            ftxui::vbox({
                ftxui::text(
                    "Your terminal is too small. Minimum requirement: "
                    + std::to_string(ui::MIN_TERMINAL_WIDTH) + " x "
                    + std::to_string(ui::MIN_TERMINAL_HEIGHT) + "."
                ),
                ftxui::paragraphAlignLeft("Please try resizing your terminal window, and then run the program again."),
            })
    }) | ftxui::borderRounded;
    auto screen = ftxui::Screen::Create(
        ftxui::Dimension::Full(),
        ftxui::Dimension::Fit(document)
    );
    ftxui::Render(screen, document);
    screen.Print();
    util::WriteToLog("Terminal is too small to launch. Exiting...", "?", "FATAL");

    exit(1);
}

//  -- Main Menu Functions -------------------------------------------------------

void getMenuOption(int& option) {
    int selected = 0;
    int prevSelected = 0; // for implementing mouse double click

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
                    ftxui::text("Select an option and press Enter, or double click an option:\n") | ftxui::hcenter | ftxui::underlined,
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
        if (event.is_mouse()
            && event.mouse().button == ftxui::Mouse::Left
            && event.mouse().motion == ftxui::Mouse::Released) {
            if (selected == prevSelected) {
                ui::appScreen.ExitLoopClosure()();
                return true;
            }
            prevSelected = selected;
        }
        return false;
    })  | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, ui::MIN_TERMINAL_WIDTH) 
        | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, ui::MIN_TERMINAL_HEIGHT)
        | ftxui::center;

    ui::appScreen.Loop(renderer);

    option = selected;
    util::WriteToLog("Menu option selected: " + std::to_string(option), "getMenuOption()");
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

    // -- Custom Game Options
    //    -- Path to custom game file (input)
    std::string options_gameMapFile = "";
    auto gameMapFileInput = ftxui::Input(&options_gameMapFile, "/enter/path/to/custom/game/file/here");
    auto gameMapFileInputComponent = wrapComponent("Game Map File", ftxui::Renderer(gameMapFileInput, [&] {
        return ftxui::vbox({
            gameMapFileInput->Render() | ftxui::flex_grow,
            ftxui::paragraphAlignLeft("Please refer to the README.md file for the correct format and syntax of the custom game file.") | ftxui::dim
        }) | ftxui::flex_grow;
    }));

    //    -- Player HP (slider, 10 - 100)
    int options_playerHP = 50;
    auto sliderOption = ftxui::SliderOption<int>();
    sliderOption.value = &options_playerHP;
    sliderOption.min = 10;
    sliderOption.max = 100;
    sliderOption.increment = 1;
    sliderOption.color_active = ftxui::Color::Green1;
    sliderOption.color_inactive = ftxui::Color::White;
    auto playerHPSlider = ftxui::Slider(sliderOption);

    auto playerHPSliderComponent = wrapComponent("Player HP", ftxui::Renderer(playerHPSlider, [&] {
        return ftxui::hbox({
            playerHPSlider->Render() | ftxui::flex_grow,
            ftxui::text((options_playerHP == 100 ? " " : "  ") + std::to_string(options_playerHP) + " / 100 ") |  ftxui::flex_shrink 
        }) | ftxui::flex_grow;
    }));

    //    -- Types of mob generated (checkboxes in frame)
    std::map<core::EntityType, std::string> mobTypeNames = {
        {core::EntityType::ZOMBIE, "Zombie"},
    };
    std::unordered_map<core::EntityType, std::string> mobTypeDescriptions = {
        {core::EntityType::ZOMBIE, "Zombies: 5 HP, 1 damage."},
    };
    std::unordered_map<core::EntityType, bool*> mobFlags = {
        {core::EntityType::ZOMBIE, new bool(true)},
    };
    auto mobCheckboxesContainer = ftxui::Container::Vertical({});
    for (const auto& mobType : mobTypeNames) {
        auto key = mobType.first;
        auto checkboxOption = ftxui::CheckboxOption();
        checkboxOption.transform = [&] (ftxui::EntryState state) {
            return ftxui::hbox({
                ftxui::text("["),
                (state.state
                    ? ftxui::text("O N") | ftxui::color(ftxui::Color::Green1) | ftxui::bold
                    : ftxui::text("OFF") | ftxui::color(ftxui::Color::Red) | ftxui::bold)
                        | (state.focused ? ftxui::inverted : ftxui::nothing),
                ftxui::text("] "),
                ftxui::text(state.label),
                ftxui::text(" "),
                state.focused || state.state
                    ? ftxui::text("[" + mobTypeDescriptions[key] + "]") | ftxui::dim
                    : ftxui::text(""),
            });
        };
        auto checkbox = ftxui::Checkbox(mobTypeNames[key], mobFlags[key], checkboxOption);
        mobCheckboxesContainer->Add(checkbox);
    }
    auto mobCheckboxesComponent = wrapComponent("Mob Types", ftxui::Renderer(mobCheckboxesContainer, [&] {
        return mobCheckboxesContainer->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 3) | ftxui::xflex_grow;
    }));

    //    -- Error message about invalid options
    bool showOptionError = false;
    std::string optionErrorMsg = "";
    auto errorMessageRenderer = ftxui::Renderer([&] {
        return ftxui::vbox({
            ftxui::text("Error occurred while seting up the game:") | ftxui::bold | ftxui::color(ftxui::Color::Red),
            ftxui::hbox({
                ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 4),
                ftxui::text(optionErrorMsg) | ftxui::color(ftxui::Color::Red),
            })
        });
    });

    // -- Container for all options
    auto customGameOptionsContainer = ftxui::Container::Vertical({
        gameMapFileInputComponent,
        playerHPSliderComponent,
        mobCheckboxesComponent
    });
    auto customGameOptionsRenderer = ftxui::Renderer(customGameOptionsContainer, [&] {
        return ftxui::vbox({
            ftxui::text("Customise your game:") | ftxui::bold,
            ftxui::separator(),
            gameMapFileInputComponent->Render(),
            ftxui::separator(),
            playerHPSliderComponent->Render(),
            ftxui::separator(),
            mobCheckboxesComponent->Render(),
        });
    }) | ftxui::border;

    // -- Back button
    auto backButton = ftxui::Button(" < Back to Menu ", [&] {
        ui::appScreen.ExitLoopClosure()();

        // Garbage collection
        for (const auto& mobType : mobFlags) {
            delete mobType.second;
        }
    });

    // -- Start button
    auto startButton = ftxui::Button(" Start Game > ", [&] {
        core::GameOptions* gameOptions = nullptr;
        static core::GameOptions defaultOptionObj;
        switch (selectedDifficulty) {
            case 0: defaultOptionObj = core::DefaultGameOptions::EASY(); gameOptions = &defaultOptionObj; break;
            case 1: defaultOptionObj = core::DefaultGameOptions::MEDIUM(); gameOptions = &defaultOptionObj; break;
            case 2: defaultOptionObj = core::DefaultGameOptions::HARD(); gameOptions = &defaultOptionObj; break;
            case 3: gameOptions = new core::GameOptions(); break;
            default: break;
        }
        util::WriteToLog("Difficulty selected: " + std::to_string(selectedDifficulty), "difficultyMenu()");

        if (selectedDifficulty == 3) {
            // ===== Here checks if the custom game file & options are valid =====
            // e.g., At least one mob type must be selected, etc.

            // check game map file
            if (options_gameMapFile.empty()) {
                setErrorMessage(optionErrorMsg, "Game file path cannot be empty.", showOptionError);
                return;
            }
            std::ifstream fin(options_gameMapFile.c_str());
            auto reader = core::ArenaReader(fin);
            if (!reader.IsSuccess()) {
                setErrorMessage(optionErrorMsg, reader.GetErrorMessage(), showOptionError);
                return;
            }
            fin.close();

            // check player HP (nothing to be checked)
            gameOptions->PlayerHp = options_playerHP;

            // check mob types
            auto mobTypes = getMobTypes(mobFlags);
            if (mobTypes.empty()) {
                setErrorMessage(optionErrorMsg, "At least one type of mobs must be enabled.", showOptionError);
                return;
            }
            gameOptions->MobTypesGenerated = mobTypes;
        }

        // Set the game options
        gameLvl_configureGameOptions(gameOptions);
        gameLvl_customMode = selectedDifficulty == 3;
        gameLvl_mainGameLoop();

        // garbage collection
        for (const auto& mobType : mobFlags) {
            delete mobType.second;
        }

        // Exit the menu and hand over to main menu for starting the game
        ui::appScreen.ExitLoopClosure()();
    });

    // -- Wrapper Box
    auto layout = ftxui::Container::Vertical({
        ftxui::Renderer([] {
            return ftxui::vbox({
                ftxui::text("Select a difficulty level:") | ftxui::center | ftxui::bold,
                ftxui::separator(),
            });
        }),
        difficultyRadioButtons | ftxui::borderDouble,
        customGameOptionsRenderer | ftxui::Maybe(&loadCustomGame),
        errorMessageRenderer | ftxui::Maybe(&showOptionError),
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

ftxui::Component wrapComponent(std::string fieldName, ftxui::Component component) {
    // field name must be less than or equal to 15 characters, otherwise truncate and add ...
    if (fieldName.length() > 15) fieldName = fieldName.substr(0, 12) + "...";
    // adds the field name to the left of the component
    return ftxui::Renderer(component, [fieldName, component] {
        return ftxui::hbox({
            ftxui::text(fieldName) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15) | ftxui::bold,
            ftxui::separator(),
            ftxui::text(" "),
            component->Render(),
            ftxui::text(" ")
        });
    });
}

//  -- Custom Game Options Functions -------------------------------------------------------

std::set<core::EntityType> getMobTypes(std::unordered_map<core::EntityType, bool*> mobFlags) {
    std::set<core::EntityType> mobTypes;
    for (const auto& mobType : mobFlags) {
        if (*mobType.second) {
            mobTypes.insert(mobType.first);
        }
    }
    return mobTypes;
}

void setErrorMessage(std::string& errorMessageRef, std::string msg, bool& showError) {
    errorMessageRef = msg;
    showError = true;
}