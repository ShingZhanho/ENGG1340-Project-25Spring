#include <fstream>
#include <string>
#include <strstream>

#include <core/arena.hpp>
#include <core/arena_reader.hpp>

namespace core {

    ArenaReader::ArenaReader(std::ifstream& fs) : file(&fs) {
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
        if (!file->is_open()) {
            errmsg = "Encountered issues opening the file.";
            return false;
        }

        arena = new Arena();
        
        bool playerFound = false;
        std::string line;

        for (int y = 0; y < ARENA_HEIGHT; ++y) {
            if (!std::getline(*file, line)) line = std::string(ARENA_WIDTH, ' ');
            if (line.length() < ARENA_WIDTH) line.append(ARENA_WIDTH - line.length(), ' ');
            for (int x = 0; x < ARENA_HEIGHT; ++x) {
                char c = line[x];
                switch (c) {
                    case 'X': // wall
                        arena->SetPixel({x, y}, new Wall({x, y}, arena));
                        break;
                    case 'P': // player
                        if (!playerFound) {
                            arena->SetPixelWithId({x, y}, new Player({x, y}, arena));
                            playerFound = true;
                            break;
                        }
                        errmsg = "Invalid file syntax. More than one \'P\' found in the file. (line " + std::to_string(y) + ", column " + std::to_string(x) + ")";
                        return false;
                    case ' ': // air
                        arena->SetPixel({x, y}, new Air({x, y}, arena));
                        break;
                    default:
                        errmsg = "Invalid file syntax. Unknown character \'" + std::string(1, c) + "\' found in the file. (line " + std::to_string(y) + ", column " + std::to_string(x) + ")";
                        return false;
                }
            }
        }

        if (!playerFound) {
            errmsg = "Invalid file syntax. Expected exactly one \'P\' in the file. None found.";
            return false;
        }
        
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
