#include <fstream>
#include <string>
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

    bool ArenaReader::parseFile_() : {
        // TODO: implement the file parsing logic
        /*
            Format & Syntax of the file:
                1. The size of the arena must be of the dimensions ARENA_WIDTH and ARENA_HEIGHT defined in core/arena.hpp.
                2. Each line in the file represents a row of the arena. Each character in the line represents a pixel.
                3. If there are more lines/characters than required, those extra lines/characters will be ignored.
                4. If there are fewer lines/characters than required, the unknown pixels will be set to air.
                5. ** The outermost layer of the arena MUST always be walls, regardless of the file content. ** (this is handled within the Arena already)
                6. The characters in the file should be mapped like this:
                    ' ' = air; 'X' = wall; 'P' = starting poing of the player;
                7. The file MUST not contain mobs or other entities, otherwise a fail should be returned.
        */
        /* there are several parameters:
        parse_success, which represents whether the file is valid and parsing can be done successfully.
        custom_width and custom_height, which represents the width and height of the user_defined map.
        The abovementioned two parameters need not be equal, larger, or smaller thatn ARENA_WIDth and ARENA_HEIGHT.
        A comparison will be made to determine whether the user defined map is greater or smaller than the
        predifined width and height.
        arena =  new Arena() will be created and extra internal walls may be created
        depending on whether the custom_width and custom_height is greater/smaller than
        ARENA_WIDTH and ARENA_HEIGHT.
        */
        bool parse_success = true;
        if (!file->is_open()) {
            parse_success = false;
        }
        else {
            int custom_width, custom_height;
            std::string mob;
            while (file >> mob) {
                if (mob != " " && mob != "X" && mob != "P"){
                    parse_success = false;
                    break;
                }
            }
            std::string line;
            custom_width = (getline(file, line)).length();
        }
        return parse_success;
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
