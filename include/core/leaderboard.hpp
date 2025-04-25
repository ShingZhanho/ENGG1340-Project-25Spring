#ifndef CORE_LEADERBOARD_HPP
#define CORE_LEADERBOARD_HPP

#include <fstream>
#include <string>

namespace core {
    
    //  Represents the leaderboard of the game.
    class Leaderboard {
        public:
            //  Constructor
            Leaderboard();
            //  Destructor
            ~Leaderboard();
            //  Represents a single entry in the leaderboard.
            //  This is a node in the linked list.
            typedef struct Entry {
                //  Constructor
                Entry(std::string name, long time, int score) : Name(name), Time(time), Score(score), Next(nullptr) { }
                //  Player's name
                std::string Name;
                //  Time of the record entry. Stored in seconds since epoch (Unix time).
                long Time;
                //  The player's score.
                int Score;
                //  Pointer to the next entry.
                Entry* Next;
            } Entry;
            //  Adds a new entry to the leaderboard.
            //  Takes three parameters: the player's name, the time of the record entry, and the player's score.
            //  Returns the index of the new entry in the leaderboard (0-based).
            int AddEntry(std::string name, long time, int score, Entry*& current);
            //  Gets the head of the leaderboard.
            //  Returns a pointer to the head of the linked list.
            Entry* GetHead() const { return head; };
            //  Checks if the object is in a valid state.
            //  Returns true if the object is valid, false otherwise.
            bool IsValid() const { return objIsValid; };

        private:
            //  Pointer to the head of the linked list.
            Entry* head = nullptr;
            //  The file stream for the leaderboard file.
            std::fstream fs;
            //  Indicates if the object is in a valid state.
            bool objIsValid = false;
    };

} // namespace core


#endif // CORE_LEADERBOARD_HPP
