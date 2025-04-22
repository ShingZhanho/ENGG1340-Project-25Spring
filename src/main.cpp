// Standard Libraries
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

// Core Components
#include <ui/common.hpp>
#include <ui/render_option.hpp>
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
std::set<core::EntityType> getMobTypes(std::map<int, bool*> mobFlags);
//  Sets the error message and shows the error message.
void setErrorMessage(std::string& errorMessage, std::string msg, bool& showError);
 
int main(void) {
    util::WriteToLog("Process started.", "main()");
    // Check if the terminal is large enough
    checkTerminalSize();

    int menuOption = 0;
    while (menuOption != 1) { // 1 = "Exit"
        getMenuOption(menuOption);
        if (menuOption == 0 ) difficultyMenu();
        util::WriteToLog("One iteration of main menu completed.", "main()");
    }

    util::WriteToLog("Reached target: [SHUTDOWN]", "main()");
    util::WriteToLog("Process completed.", "main()");

    return 0;
}

void checkTerminalSize() {// Check if the terminal is large enough
    util::WriteToLog("Checking terminal size...", "checkTerminalSize()");
    util::WriteToLog("Required size: " + std::to_string(ui::MIN_TERMINAL_WIDTH) + " x " + std::to_string(ui::MIN_TERMINAL_HEIGHT) + ". Actual size: " + std::to_string(ui::ACTUAL_TERMINAL_WIDTH) + " x " + std::to_string(ui::ACTUAL_TERMINAL_HEIGHT) + ".");

    if (ui::ACTUAL_TERMINAL_WIDTH >= ui::MIN_TERMINAL_WIDTH 
     && ui::ACTUAL_TERMINAL_HEIGHT >= ui::MIN_TERMINAL_HEIGHT) {
        util::WriteToLog("Terminal size check passed.", "checkTerminalSize()");
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
    util::WriteToLog("Terminal is too small to launch. Exiting...", "checkTerminalSize()", "FATAL");

    exit(1);
}

//  -- Main Menu Functions -------------------------------------------------------

void getMenuOption(int& option) {
    util::WriteToLog("Starting main menu...", "getMenuOption()");

    int selected = 0;
    int prevSelected = -1; // for implementing mouse double click

    std::vector<std::string> menuOptions = {
        "Start Game",
        "Exit"
    };
    std::vector<std::string> descriptionStrings = {
        " Starts the game.",
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
            e |= (entryState.index == 1) ? ftxui::bgcolor(ftxui::Color::Red) : ftxui::bgcolor(ftxui::Color::Green1);
        } 
        if (entryState.focused) {
            e |= (entryState.index == 1) ? ftxui::color(ftxui::Color::Red) : ftxui::color(ftxui::Color::Green1);
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
    util::WriteToLog("Exiting main menu...", "getMenuOption()");
}

//  -- Game Difficulty Menu Functions -------------------------------------------------------

void difficultyMenu() {
    // -- Game Difficulty Menu (Radio Buttons)
    int selectedDifficulty = 0;
    bool loadCustomGame = false;
    bool showHelp = true;
    auto radioOption = ftxui::RadioboxOption();
    radioOption.on_change = [&] { loadCustomGame = (selectedDifficulty == 3); showHelp = !loadCustomGame; };
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
    auto playerHpSliderOption = ftxui::SliderOption<int>();
    playerHpSliderOption.value = &options_playerHP;
    playerHpSliderOption.min = 10;
    playerHpSliderOption.max = 100;
    playerHpSliderOption.increment = 1;
    playerHpSliderOption.color_active = ftxui::Color::Green1;
    playerHpSliderOption.color_inactive = ftxui::Color::White;
    auto playerHPSlider = ftxui::Slider(playerHpSliderOption);

    auto playerHPSliderComponent = wrapComponent("Player HP", ftxui::Renderer(playerHPSlider, [&] {
        return ftxui::hbox({
            playerHPSlider->Render() | ftxui::flex_grow,
            ftxui::text((options_playerHP == 100 ? " " : "  ") + std::to_string(options_playerHP) + " / 100 ") |  ftxui::flex_shrink 
        }) | ftxui::flex_grow;
    }));

    //    -- Types of mob generated (checkboxes in frame)
    std::map<int, std::string> mobTypeNames = {
        {static_cast<int>(core::EntityType::ZOMBIE), "Zombie"},
        {static_cast<int>(core::EntityType::TROLL), "Troll"},
        {static_cast<int>(core::EntityType::BABY_ZOMBIE), "Baby Zombie"},
        {static_cast<int>(core::EntityType::MONSTER), "Monster"},
        {static_cast<int>(core::EntityType::BOSS), "Boss"},
    };
    std::map<int, std::string> mobTypeDescriptions = {
        {static_cast<int>(core::EntityType::ZOMBIE), "1 HP, 1 damage, 1 point, moves every 1 second."},
        {static_cast<int>(core::EntityType::TROLL), "5 HP, 2 damage, 5 points, moves every 2 seconds."},
        {static_cast<int>(core::EntityType::BABY_ZOMBIE), "1 HP, 1 damage, 2 points, moves every 0.5 seconds."},
        {static_cast<int>(core::EntityType::MONSTER), "10 HP, 5 damage, 10 points, moves every 0.5 seconds."},
        {static_cast<int>(core::EntityType::BOSS), "1000 HP, 50 damage, 100 points, moves every 4 seconds."},
    };
    std::map<int, ui::RenderOption> mobTypeAppearance = {
        {static_cast<int>(core::EntityType::ZOMBIE), core::EntityRenderOptions::ZombieRenderOption()},
        {static_cast<int>(core::EntityType::TROLL), core::EntityRenderOptions::TrollRenderOption()},
        {static_cast<int>(core::EntityType::BABY_ZOMBIE), core::EntityRenderOptions::BabyZombieRenderOption()},
        {static_cast<int>(core::EntityType::MONSTER), core::EntityRenderOptions::MonsterRenderOption()},
        {static_cast<int>(core::EntityType::BOSS), core::EntityRenderOptions::BossRenderOption()},
    };
    std::map<int, bool*> mobFlags = {
        {static_cast<int>(core::EntityType::ZOMBIE), new bool(true)},
        {static_cast<int>(core::EntityType::TROLL), new bool(false)},
        {static_cast<int>(core::EntityType::BABY_ZOMBIE), new bool(false)},
        {static_cast<int>(core::EntityType::MONSTER), new bool(false)},
        {static_cast<int>(core::EntityType::BOSS), new bool(false)},
    };
    auto mobCheckboxesContainer = ftxui::Container::Vertical({});
    for (const auto& mobType : mobTypeNames) {
        auto checkboxOption = ftxui::CheckboxOption();
        checkboxOption.transform = [&] (ftxui::EntryState state) {
            auto key = mobType.first;
            return ftxui::hbox({
                ftxui::text("["),
                (state.state
                    ? ftxui::text("O N") | ftxui::color(ftxui::Color::Green1) | ftxui::bold
                    : ftxui::text("OFF") | ftxui::color(ftxui::Color::Red) | ftxui::bold)
                        | (state.focused ? ftxui::inverted : ftxui::nothing),
                ftxui::text("] "),
                ftxui::text(state.label),
                ftxui::text(" ("),
                mobTypeAppearance[key].Render(),
                ftxui::text(") "),
                state.focused || state.state
                    ? ftxui::text("[" + mobTypeDescriptions[key] + "]") | ftxui::dim
                    : ftxui::text(""),
            });
        };
        auto checkbox = ftxui::Checkbox(mobTypeNames[mobType.first], mobFlags[mobType.first], checkboxOption);
        mobCheckboxesContainer->Add(checkbox);
    }
    auto mobCheckboxesComponent = wrapComponent("Mob Types", ftxui::Renderer(mobCheckboxesContainer, [&] {
        return mobCheckboxesContainer->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 3) | ftxui::xflex_grow;
    }));

    //      -- Max Mobs (slider)
    int options_maxMobs = 15;
    auto maxMobsSliderOption = ftxui::SliderOption<int>();
    maxMobsSliderOption.value = &options_maxMobs;
    maxMobsSliderOption.min = 10;
    maxMobsSliderOption.max = 50;
    maxMobsSliderOption.increment = 1;
    maxMobsSliderOption.color_active = ftxui::Color::Green1;
    maxMobsSliderOption.color_inactive = ftxui::Color::White;
    auto maxMobsSlider = ftxui::Slider(maxMobsSliderOption);

    auto maxMobsSliderComponent = wrapComponent("Max. Mobs", ftxui::Renderer(maxMobsSlider, [&] {
        return ftxui::hbox({
            maxMobsSlider->Render() | ftxui::flex_grow,
            ftxui::text(" " +std::to_string(options_maxMobs) + " / 50 ") |  ftxui::flex_shrink 
        }) | ftxui::flex_grow;
    }));

    //      -- Mob Spawn Interval (slider, 2 - 15)
    int options_mobSpawnInterval = 5;
    auto mobSpawnIntervalSliderOption = ftxui::SliderOption<int>();
    mobSpawnIntervalSliderOption.value = &options_mobSpawnInterval;
    mobSpawnIntervalSliderOption.min = 2;
    mobSpawnIntervalSliderOption.max = 15;
    mobSpawnIntervalSliderOption.increment = 1;
    mobSpawnIntervalSliderOption.color_active = ftxui::Color::Green1;
    mobSpawnIntervalSliderOption.color_inactive = ftxui::Color::White;
    auto mobSpawnIntervalSlider = ftxui::Slider(mobSpawnIntervalSliderOption);

    auto mobSpawnIntervalSliderComponent = wrapComponent("Spawn Interval", ftxui::Renderer(mobSpawnIntervalSlider, [&] {
        return ftxui::hbox({
            mobSpawnIntervalSlider->Render() | ftxui::flex_grow,
            ftxui::text((options_mobSpawnInterval < 9 ? "  " : " ") + std::to_string(options_mobSpawnInterval) + " / 15 seconds ") |  ftxui::flex_shrink 
        }) | ftxui::flex_grow;
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

    auto gameHelpRenderer = ftxui::Renderer([] {
        return ftxui::vbox({
            ftxui::text(" OBJECTS IN THE GAME") | ftxui::bold,
            ftxui::separator(),
            ftxui::hbox({
                ftxui::text(" BASIC OBJECTS: ") | ftxui::bold,
                ftxui::separator(),
                core::EntityRenderOptions::PlayerRenderOption().Render(), ftxui::text(" - Player (you)"),
                ftxui::separator(),
                core::EntityRenderOptions::WallRenderOption().Render(), ftxui::text(" - Wall (obstacle)"),
                ftxui::separator(),
                core::EntityRenderOptions::AirRenderOption().Render(), ftxui::text(" - Air (empty space)"),
                ftxui::separator(),
                core::EntityRenderOptions::PlayerBulletRenderOption().Render(), ftxui::text(" - Player Bullet"),
            }),
            ftxui::separator(),
            ftxui::hbox({
                ftxui::text(" MOBS: ") | ftxui::bold,
                ftxui::separator(),
                core::EntityRenderOptions::ZombieRenderOption().Render(), ftxui::text(" - Zombie"),
                ftxui::separator(),
                core::EntityRenderOptions::TrollRenderOption().Render(), ftxui::text(" - Troll"),
                ftxui::separator(),
                core::EntityRenderOptions::BabyZombieRenderOption().Render(), ftxui::text(" - Baby Zombie"),
                ftxui::separator(),
                core::EntityRenderOptions::MonsterRenderOption().Render(), ftxui::text(" - Monster"),
                ftxui::separator(),
                core::EntityRenderOptions::BossRenderOption().Render(), ftxui::text(" - Boss")
            }),
            ftxui::separator(),
            ftxui::hbox({
                ftxui::text(" ITEMS: ") | ftxui::bold,
                ftxui::separator(),
                ftxui::vbox({
                    ftxui::text(" (Be careful! Mobs can also pick up these collectibles and use them against you!)"),
                    ftxui::separator(),
                    ftxui::hbox({
                        core::EntityRenderOptions::EnergyDrinkRenderOption(5).Render(), ftxui::text(" - Energy Drink (+HP)"),
                        ftxui::separator(),
                        core::EntityRenderOptions::StrengthPotionRenderOption(5).Render(), ftxui::text(" - Strength Potion (+damage)"),
                        ftxui::separator(),
                        core::EntityRenderOptions::ShieldRenderOption().Render(), ftxui::text(" - Shield (temporary invincibility)"),
                    })
                }) | ftxui::xflex_grow
            }),
            
        }) | ftxui::border | ftxui::xflex_grow;
    });

    // -- Container for all options
    auto customGameOptionsContainer = ftxui::Container::Vertical({
        gameMapFileInputComponent,
        playerHPSliderComponent,
        mobCheckboxesComponent,
        maxMobsSliderComponent,
        mobSpawnIntervalSliderComponent,
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
            ftxui::separator(),
            maxMobsSliderComponent->Render(),
            ftxui::separator(),
            mobSpawnIntervalSliderComponent->Render(),
        });
    }) | ftxui::border;

    // -- Garbage collection
    auto cleanup = [&] {
        util::WriteToLog("Cleaning up...", "difficultyMenu()");
        for (const auto& mobType : mobFlags) {
            delete mobType.second;
        }
    };

    // -- Back button
    auto backButton = ftxui::Button(" < Back to Menu ", [&] {
        // Garbage collection
        cleanup();

        util::WriteToLog("Back button pressed. Game start cancelled. Exiting difficulty menu...", "difficultyMenu()");
        ui::appScreen.ExitLoopClosure()();
    });

    // -- Start button
    auto startButton = ftxui::Button(" Start Game > ", [&] {
        util::WriteToLog("Start button pressed. Setting up game...", "difficultyMenu()");
        static core::GameOptions* gameOptions;
        gameOptions = nullptr;
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
            util::WriteToLog("Custom game selected. Validating options...", "difficultyMenu()");

            // check game map file
            if (options_gameMapFile.empty()) {
                setErrorMessage(optionErrorMsg, "Game file path cannot be empty.", showOptionError);
                util::WriteToLog("Game setup failed. Reason: could not open file " + options_gameMapFile, "difficultyMenu()", "ERROR");
                return;
            }
            std::ifstream fin(options_gameMapFile.c_str());
            auto reader = core::ArenaReader(fin);
            if (!reader.IsSuccess()) {
                setErrorMessage(optionErrorMsg, reader.GetErrorMessage(), showOptionError);
                util::WriteToLog("Game setup failed. Reason: " + reader.GetErrorMessage(), "difficultyMenu()", "ERROR");
                return;
            }
            fin.close();
            gameOptions->GameArena = reader.GetArena();

            // check player HP (nothing to be checked)
            gameOptions->PlayerHp = options_playerHP;

            // check mob types
            auto mobTypes = getMobTypes(mobFlags);
            if (mobTypes.empty()) {
                setErrorMessage(optionErrorMsg, "At least one type of mobs must be enabled.", showOptionError);
                util::WriteToLog("Game setup failed. Reason: no mob types selected.", "difficultyMenu()", "ERROR");
                return;
            }
            gameOptions->MobTypesGenerated = mobTypes;

            // check max mobs (nothing to be checked)
            gameOptions->MaxMobs = options_maxMobs;

            // check spawn interval (nothing to be checked)
            gameOptions->MobSpawnInterval = options_mobSpawnInterval * 50; // 50 ticks = 1 second
        }

        // Set the game options
        gameLvl_configureGameOptions(gameOptions);
        gameLvl_customMode = selectedDifficulty == 3;
        gameLvl_mainGameLoop();

        util::WriteToLog("Game loop exited. Cleaning up...", "difficultyMenu()");

        // garbage collection
        cleanup();

        // Exit the menu
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
        gameHelpRenderer | ftxui::Maybe(&showHelp),
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

    util::WriteToLog("Game difficulty menu exited.", "difficultyMenu()");
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

std::set<core::EntityType> getMobTypes(std::map<int, bool*> mobFlags) {
    std::set<core::EntityType> mobTypes;
    for (const auto& mobType : mobFlags) {
        if (*mobType.second) {
            mobTypes.insert((core::EntityType) mobType.first);
        }
    }
    return mobTypes;
}

void setErrorMessage(std::string& errorMessageRef, std::string msg, bool& showError) {
    errorMessageRef = msg;
    showError = true;
}