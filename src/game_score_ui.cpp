#include "game_score_ui.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <core/leaderboard.hpp>
#include <ui/common.hpp>

#include <ctime>
#include <string>

inline static std::string getOrdSuffix(int number) {
    if (number % 100 >= 11 && number % 100 <= 13) {
        return "th";
    }
    switch (number % 10) {
        case 1: return "st";
        case 2: return "nd";
        case 3: return "rd";
        default: return "th";
    }
}

void gameScore_displayScore(int score) {
    // Leaderboard object
    auto leaderboard = core::Leaderboard();
    std::time_t currentTime = std::time(nullptr);
    std::string playerName = "Mysterious_Warrior";
    int rank = leaderboard.AddEntry(playerName, currentTime, score) + 1;

    // Text input for entering player's name
    auto inputOptions = ftxui::InputOption::Default();
    inputOptions.placeholder = "Enter your name here...";
    inputOptions.content = &playerName;
    inputOptions.multiline = false;
    auto playerNameInput = ftxui::Input(inputOptions);

    // Button to return to the main menu
    auto returnButton = ftxui::Button("< Return to Menu", [&] {
        if (playerName.length() > 20) playerName = playerName.substr(0, 20);
        std::replace(playerName.begin(), playerName.end(), ' ', '_');
        leaderboard.GetEntry(rank - 1)->Name = playerName.empty() ? "Mysterious_Warrior" : playerName;
        ui::appScreen.ExitLoopClosure()();
    });

    auto container = ftxui::Container::Vertical({
        ftxui::Renderer([&] {
            return ftxui::vbox({
                    ftxui::text("GAME OVER!") | ftxui::center | ftxui::color(ftxui::Color::Red),
                    ftxui::separatorDouble(),
                    ftxui::text(""), ftxui::text(""),
                    ftxui::text("Challenger, in this battle, you have scored:") | ftxui::center,
                    ftxui::text(""), ftxui::text(""),
                    ftxui::hbox({
                        ftxui::filler(),
                        ftxui::text(std::to_string(score))  | ftxui::color(ftxui::Color::Green)
                                                            | ftxui::bold
                                                            | ftxui::borderRounded,
                        ftxui::filler(),
                    }),
                    ftxui::text(""), ftxui::text(""),
                    ftxui::text("You are ranked:") | ftxui::center,
                    ftxui::text(""), ftxui::text(""),
                    ftxui::hbox({
                        ftxui::filler(),
                        ftxui::text(std::to_string(rank) + getOrdSuffix(rank) + " place")
                            | ftxui::center
                            | ftxui::color(ftxui::Color::Yellow)
                            | ftxui::bold
                            | ftxui::borderRounded
                            | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15),
                        ftxui::filler(),
                    }),
                    ftxui::text(""), ftxui::text(""),
                    ftxui::separator(),
                    ftxui::text(""), ftxui::text(""),
                    ftxui::text("Announce your name to the world,") | ftxui::center,
                    ftxui::text("or leave it blank to remain a mystery:") | ftxui::center,
                    ftxui::text(""), ftxui::text(""),
            });
        }),
        playerNameInput | ftxui::borderDashed,
        ftxui::Renderer([] { return ftxui::filler() | ftxui::flex_grow; }),
        returnButton | ftxui::center,
    })  | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, ui::MIN_TERMINAL_WIDTH)
        | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, ui::MIN_TERMINAL_HEIGHT)
        | ftxui::borderDouble
        | ftxui::center;
    
    ui::appScreen.Loop(container);
}