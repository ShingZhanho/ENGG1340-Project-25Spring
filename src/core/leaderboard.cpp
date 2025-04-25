#include <core/leaderboard.hpp>
#include <util/log.hpp>

#include <sstream>
#include <string>

namespace core {
    Leaderboard::Leaderboard() {
        //  Each line will have the following format:
        //       <name (std::string)> <time (long)> <score (int)>
        //  The player's name will not contain the whitespace character ' '.

        //  Open file
        std::string file = "./res/leaderboard.txt", line = "", word = "";
        fs.open(file.c_str(), std::ios::in);
        if (!fs.is_open()){
            util::WriteToLog("Leaderboard file does not exist. Creating a new one.", "Leaderboard::Leaderboard()");
            fs.open(file.c_str(), std::ios::out | std::ios::trunc);
        }
        if (!fs.is_open()){
            util::WriteToLog("Failed to open leaderboard file.", "Leaderboard::Leaderboard()", "ERROR");
            return;
        }
        
        //  Read the file and build the linked list
        while (std::getline(fs, line)) {
            std::istringstream iss(line);
            std::string name;
            long time = 0;
            int score = 0;

            // skip invalid lines
            if (line.empty()) continue;
            if (iss >> name >> time >> score || !iss.eof()) {
                if (iss.fail()) {
                    util::WriteToLog("An invalid line was found in the leaderboard file: " + line, "Leaderboard::Leaderboard()", "WARNING");
                    iss.clear();
                    continue;
                }
                AddEntry(name, time, score);
            }
        }

        //  Close the file
        fs.close();

        objIsValid = true;
    }

    Leaderboard::~Leaderboard() {
        //  TODO: Implement save logic.
        //  Clear the file first, and then write the entries in the linked list to the file.
        //  Use the format specified above.

        //  TODO: Implement the delete logic.
        //  Then, delete all the entries in the linked list one by one.

    }

    int Leaderboard::AddEntry(std::string name, long time, int score) {
        return 0;
        //  TODO: Implement this function
    }
}
