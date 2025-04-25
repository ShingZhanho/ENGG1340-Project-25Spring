#include <core/leaderboard.hpp>
#include <util/log.hpp>

#include <ctime>
#include <sstream>
#include <string>

namespace core {
    Leaderboard::Leaderboard() {
        //  Each line will have the following format:
        //       <name (std::string)> <time (long)> <score (int)>
        //  The player's name will not contain the whitespace character ' '.

        //  Open file
        std::string file = "./leaderboard.txt";
        std::string line;
        fs.open(file.c_str(), std::ios::in);
        if (!fs.is_open()){
            util::WriteToLog("Leaderboard file does not exist. Creating a new one.", "Leaderboard::Leaderboard()");
            fs.open(file.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
        }
        if (!fs.is_open()){
            util::WriteToLog("Failed to open leaderboard file.", "Leaderboard::Leaderboard()", "ERROR");
            return;
        }
        
        //  Read the file and build the linked list
        while (std::getline(fs, line)) {
            std::istringstream iss(line);
            std::string name;
            int timeInt = 0;
            std::time_t time = 0;
            int score = 0;

            // skip invalid lines
            if (line.empty()) continue;
            if (iss >> name >> timeInt >> score) {
                time = static_cast<std::time_t>(timeInt);
                AddEntry(name, time, score);
            } else {
                util::WriteToLog("An invalid line was found in the leaderboard file: " + line, "Leaderboard::Leaderboard()", "WARNING");
            }
        }
        //  Close the file
        fs.close();
        objIsValid = true;
    }

    Leaderboard::~Leaderboard() {
        Entry* current = head, * prev = nullptr;
        std::string file = "./leaderboard.txt";
        std::fstream fout(file.c_str(), std::ios::out | std::ios::trunc);
        while (current != nullptr) {
            fout << current->Name << " " << std::to_string(current->Time) << " " << std::to_string(current->Score) << std::endl;
            prev = current;
            current = current->Next;
            delete prev;
        }
        fout.close();
        head = nullptr;
        objIsValid = false;
    }

    int Leaderboard::AddEntry(std::string name, std::time_t time, int score) {
        Entry* current = head;
        Entry* prev = nullptr;
        Entry* entry = new Entry(name, time, score);
        int index = 0;

        // Find the correct position to insert
        while (current != nullptr) {
            if (score > current->Score ||
                (score == current->Score && time > current->Time)) {
                break;
            }
            prev = current;
            current = current->Next;
            ++index;
        }

        // Insert at head
        if (prev == nullptr) {
            entry->Next = head;
            head = entry;
            return 0;
        }

        // Insert in the middle or end
        entry->Next = current;
        prev->Next = entry;
        return index;   
    }

    Leaderboard::Entry* Leaderboard::GetEntry(int index) const {
        Entry* current = head;
        int i = 0;

        if (index == 0 && head != nullptr) return head;

        while (current != nullptr && i != index) {
            current = current->Next;
            ++i;
        }

        return (i == index) ? current : nullptr;
    }
}
