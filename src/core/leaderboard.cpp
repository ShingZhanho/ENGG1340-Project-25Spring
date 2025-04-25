#include <core/leaderboard.hpp>
#include <util/log.hpp>

#include <iostream>
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

        Entry* current = head;
        
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
                AddEntry(name, time, score, current);
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
        Entry* current = head;
        std::string file = "./res/leaderboard.txt", word = "";
        std::fstream fout(file, std::ios::out | std::ios::trunc);
        while (current != nullptr){
            fout << current->Name << " " << current->Time << " " << current->Score << std::endl;
            current = current->Next;
        }
        fout.close();
        //  TODO: Implement the delete logic.
        //  Then, delete all the entries in the linked list one by one.
        current = head;
        while (current != nullptr){
            Entry* next = current->Next;
            delete current;
            current = next;
        }
        head = nullptr;
    }

    int Leaderboard::AddEntry(std::string name, long time, int score, Entry*& current) {
        Entry* entry = new Entry(name, time, score);
        // linked list is empty (add the first non-empty line of the file)
        if (current == head){
            current = entry;
            head = entry;
            util::WriteToLog("Successfully created the first entry in linked list of Leaderboard.", "Leaderboard::AddEntry()"); 
            return 0;
        }
        // linked list is not empty
        else{
            current->Next = entry;
            current = current->Next;
            // find the index of this line record
            int i = 0;
            Entry* index = head;
            while (index != current){
                i++;
                index = index->Next;
            }
            util::WriteToLog("Successfully created an entry in linked list of Leaderboard. Its index is " + i, "Leaderboard::AddEntry()"); 
            return i;
        }
    }
}
