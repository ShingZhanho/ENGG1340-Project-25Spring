#include <core/leaderboard.hpp>
#include <sstream>
#include <iostream>
#include <string>

namespace core {
    Leaderboard::Leaderboard() {
        //  TODO: Implement this function
        //  Use the file stream object fs to open the leaderboard file (leaderboard.txt) in read/write mode.
        //  If the file does not exist, create it.
        //  Read the file and build the linked list. The head of the file is head.
        //  Each line will have the following format:
        //       <name (std::string)>;<time (long)>;<score (int)>
        //  The player's name will not contain the character ';'.
        std::string file = "leaderboard.txt", line = "", word = "";
        fs.open(file, std::ios::in | std::ios::out | std::ios::binary);
        if (!fs.is_open()){
            std::cout << "Failed to open or create " << file << "." << std::endl;
        }
        else{
            // Check whether file is empty.
            fs.seekg(0, std::ios::beg);
            std::streampos startPos = fs.tellg();
            fs.seekg(0, std::ios::end);
            std::streampos endPos = fs.tellg();
            if (endPos == startPos){
                std::cout << file << " is empty." << std::endl;
            }
            // File is not empty.
            else{
                Entry* current = head;
                while (std::getline(fs, line)){
                    std::string name = "";
                    long time;
                    int score = 0;
                    int attribute = 0;
                    std::istringstream word_in(line);
                    while (std::getline(word_in, word)){
                        switch (attribute){
                            // There are a total of three attributes in each line in the file.
                            case 0: name = word; break;
                            case 1: time = std::stol(word); break;
                            case 2: score = std::stoi(word); break;
                        }
                        attribute++;
                    }
                    Entry* entry = new Entry(name, time, score);
                    entry->Next = nullptr;
                    // Linked list is empty (Note that this does not mean the file is empty!!!).
                    if (current == nullptr){
                        current = entry;
                        head = entry;
                    }
                    // Linked list is not empty.
                    else{
                        current->Next = entry;
                        current = current->Next;
                    }
                }
            }
        }
    }

    Leaderboard::~Leaderboard() {
        //  TODO: Implement save logic.
        //  Clear the file first, and then write the entries in the linked list to the file.
        //  Use the format specified above.

        fs.close();

        //  TODO: Implement the delete logic.
        //  Then, delete all the entries in the linked list one by one.

    }

    int Leaderboard::AddEntry(std::string name, long time, int score) {
        return 0;
        //  TODO: Implement this function
    }

    Leaderboard::Entry* Leaderboard::GetHead() {
        return head;
    }
}
