#include <core/leaderboard.hpp>

namespace core {
    Leaderboard::Leaderboard() {
        //  TODO: Implement this function
        //  Use the file stream object fs to open the leaderboard file (leaderboard.txt) in read/write mode.
        //  If the file does not exist, create it.
        //  Read the file and build the linked list. The head of the file is head.
        //  Each line will have the following format:
        //       <name (std::string)>;<time (long)>;<score (int)>
        //  The player's name will not contain the character ';'.
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