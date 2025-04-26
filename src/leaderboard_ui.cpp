#include "leaderboard_ui.hpp"
#include <util/log.hpp>
#include <ui/common.hpp>
#include <ftxui/component/component.hpp>
#include <core/leaderboard.hpp>

#include <vector>
#include <iomanip>
#include <string>
#include <sstream>

// Function to get leaderboard entries for a specific mode
// int mode: 0 = Easy, 1 = Medium, 2 = Hard, 3 = Custom
std::vector<std::string> getEntries(int mode) {
    std::vector<std::string> entries;
    core::Leaderboard leaderboard(mode);
    core::Leaderboard::Entry* entry = leaderboard.GetHead();
    int ctr = 0;
    while (entry != nullptr) {
        std::stringstream ss;
        ss << "          | ";
        ss << std::right << std::setw(4) << ++ctr << " | ";
        ss << std::left << std::setw(20) << entry->Name << " | ";
        ss << std::right << std::setw(5) << entry->Score;
        ss << " |";
        entries.push_back(ss.str());
        entry = entry->Next;
    }
    if (entries.empty()) {
        entries.push_back("          < NO DATA YET >");
    }
    return entries;
}

void leaderboardUI() {
    util::WriteToLog("Starting leaderboard UI...", "leaderboardUI()");

    std::vector<std::string> leaderboardTabs = {
        " [👻] EASY Mode ", " [😈] MEDIUM Mode ", " [👹] HARD Mode ", " [🔧] CUSTOM Mode "
    };
    int selectedTab = 0;
    auto tabToggle = ftxui::Toggle(&leaderboardTabs, &selectedTab);

    // prepare the leaderboard data
    auto menuOption = ftxui::MenuOption::Vertical();
    menuOption.entries_option.transform = [] (ftxui::EntryState entryState) {
        ftxui::Element e = ftxui::text(entryState.label);
        if (entryState.index == 0) e |= ftxui::color(ftxui::Color::Gold1);
        else if (entryState.index == 1) e |= ftxui::color(ftxui::Color::White);
        else if (entryState.index == 2) e |= ftxui::color(ftxui::Color::DarkOrange3);
        else e |= ftxui::color(ftxui::Color::Grey42);
        if (entryState.focused) e = e | ftxui::bold | ftxui::inverted;
        return e;
    };
    std::vector<std::vector<std::string>> leaderboardEntries = {
        getEntries(0), // Easy
        getEntries(1), // Medium
        getEntries(2), // Hard
        getEntries(3)  // Custom
    };
    auto leaderboardHeader = ftxui::text("          | RANK |         NAME         | SCORE |");
    std::vector<int> dummies = { 0, 0, 0, 0 };
    auto leaderboardEasyContent = ftxui::Menu(&leaderboardEntries[0], &dummies[0], menuOption);
    auto leaderboardEasyContentRenderer = ftxui::Renderer(leaderboardEasyContent, [&] {
        return ftxui::vbox({
            leaderboardHeader,
            leaderboardEasyContent->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30),
        });
    });
    auto leaderboardMediumContent = ftxui::Menu(&leaderboardEntries[1], &dummies[1], menuOption);
    auto leaderboardMediumContentRenderer = ftxui::Renderer(leaderboardMediumContent, [&] {
        return ftxui::vbox({
            leaderboardHeader,
            leaderboardMediumContent->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30),
        });
    });
    auto leaderboardHardContent = ftxui::Menu(&leaderboardEntries[2], &dummies[2], menuOption);
    auto leaderboardHardContentRenderer = ftxui::Renderer(leaderboardHardContent, [&] {
        return ftxui::vbox({
            leaderboardHeader,
            leaderboardHardContent->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30),
        });
    });
    auto leaderboardCustomContent = ftxui::Menu(&leaderboardEntries[3], &dummies[3], menuOption);
    auto leaderboardCustomContentRenderer = ftxui::Renderer(leaderboardCustomContent, [&] {
        return ftxui::vbox({
            leaderboardHeader,
            leaderboardCustomContent->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30),
        });
    });

    auto tabContainer = ftxui::Container::Tab(
        { leaderboardEasyContentRenderer, leaderboardMediumContentRenderer, leaderboardHardContentRenderer, leaderboardCustomContentRenderer },
        &selectedTab
    );

    auto container = ftxui::Container::Vertical({
        tabToggle,
        tabContainer
    });

    auto renderer = ftxui::Renderer(container, [&] {
        return ftxui::vbox({
            ftxui::text("LEADERBOARD") | ftxui::center | ftxui::bold,
            ftxui::separatorDouble(),
            tabToggle->Render(),
            ftxui::separator(),
            tabContainer->Render(),
            ftxui::filler(),
            ftxui::separator(),
            ftxui::text("Press ESC to return to the menu.") | ftxui::center
        }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, ui::MIN_TERMINAL_WIDTH - 2) 
            | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, ui::MIN_TERMINAL_HEIGHT - 2)
            | ftxui::borderDouble
            | ftxui::center;
    })  | ftxui::CatchEvent([&] (ftxui::Event event) {
            if (event == ftxui::Event::Escape) {
                ui::appScreen.ExitLoopClosure()();
                return true;
            } else if (event == ftxui::Event::ArrowLeft) {
                selectedTab -= selectedTab == 0 ? 0 : 1;
                tabContainer->OnEvent(ftxui::Event::ArrowLeft);
                return true;
            } else if (event == ftxui::Event::ArrowRight) {
                selectedTab += selectedTab == 3 ? 0 : 1;
                tabContainer->OnEvent(ftxui::Event::ArrowRight);
                return true;
            }
            return false;
        });

    ui::appScreen.Loop(renderer);
}