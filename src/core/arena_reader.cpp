#include <fstream>
#include <string>
#include <sstream>

#include <core/arena.hpp>
#include <core/arena_reader.hpp>
#include <util/log.hpp>

namespace core {

    ArenaReader::ArenaReader(std::ifstream& fs) : file(&fs) {
        util::WriteToLog("Constructing ArenaReader", "ArenaReader::ArenaReader()");
        success = parseFile_();
    }

    ArenaReader::~ArenaReader() {
        // file stream is closed by the code that created it
        if (GetArena() != nullptr) {
            delete arena;
            arena = nullptr;
        }
    }

    bool ArenaReader::parseFile_() {
        util::WriteToLog("Parsing arena file...", "ArenaReader::parseFile_()");

        if (!file->is_open()) {
            util::WriteToLog("File stream is not open.", "ArenaReader::parseFile_()", "ERROR");
            errmsg = "Encountered issues opening the file.";
            return false;
        }

        arena = new Arena();
        
        bool playerFound = false;
        std::string line;

        for (int y = 0; y < ARENA_HEIGHT; ++y) {
            if (!std::getline(*file, line)) line = std::string(ARENA_WIDTH, ' ');
            if (line.length() < ARENA_WIDTH) line.append(ARENA_WIDTH - line.length(), ' ');
            if (line.length() > ARENA_WIDTH) line = line.substr(0, ARENA_WIDTH);
            util::WriteToLog("Parsing line " + std::to_string(y) + ": " + line, "ArenaReader::parseFile_()");
            for (int x = 0; x < ARENA_WIDTH; ++x) {
                char c = line[x];
                switch (c) {
                    case 'X': // wall
                        arena->SetPixel({x, y}, new Wall({x, y}, arena));
                        break;
                    case 'P': // player
                        if (!playerFound) {
                            util::WriteToLog("Player found at (" + std::to_string(x) + ", " + std::to_string(y) + ")", "ArenaReader::parseFile_()");
                            static Player* player = new Player({x, y}, arena, 0);
                            arena->SetPixelWithId({x, y}, player); // player HP will be set in InitialiseEventHandler
                            playerFound = true;
                            break;
                        }
                        util::WriteToLog("Invalid file syntax. More than one \'P\' found in the file.", "ArenaReader::parseFile_()", "ERROR");
                        errmsg = "Invalid file syntax. More than one \'P\' found in the file. (line " + std::to_string(y) + ", column " + std::to_string(x) + ")";
                        return false;
                    case ' ': // air
                        arena->SetPixel({x, y}, new Air({x, y}, arena));
                        break;
                    default:
                        util::WriteToLog("Invalid file syntax. Unknown character \'" + std::string(1, c) + "\' found in the file.", "ArenaReader::parseFile_()", "ERROR");
                        errmsg = "Invalid file syntax. Unknown character \'" + std::string(1, c) + "\' found in the file. (line " + std::to_string(y) + ", column " + std::to_string(x) + ")";
                        return false;
                }
            }
        }

        if (!playerFound) {
            util::WriteToLog("Invalid file syntax. No \'P\' found in the file.", "ArenaReader::parseFile_()", "ERROR");
            errmsg = "Invalid file syntax. Expected exactly one \'P\' in the file. None found.";
            return false;
        }

        util::WriteToLog("Arena file parsed successfully.", "ArenaReader::parseFile_()");
        return true;
    }

    Arena* ArenaReader::GetArena() {
        return success ? arena : nullptr;
    }

    std::string ArenaReader::GetErrorMessage() {
        return success ? "" : errmsg;
    }

    bool ArenaReader::IsSuccess() {
        return success;
    }

}
