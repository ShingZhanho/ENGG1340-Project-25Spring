#include <fstream>

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
        errmsg = "File parsing not implemented yet.";
        return false;
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