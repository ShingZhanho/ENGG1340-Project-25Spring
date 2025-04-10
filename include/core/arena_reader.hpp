#ifndef CORE_ARENA_READER_HPP
#define CORE_ARENA_READER_HPP

#include <fstream>

#include <core/arena.hpp>

namespace core {

    //  The ArenaReader class is responsible for reading the arena from a file.
    class ArenaReader {
        public:
            //  Constructor
            ArenaReader(std::ifstream& file);

            //  Gets a pointer to the constructed arena object if successful.
            //  Otherwise, returns nullptr.
            Arena* GetArena();

            std::string GetErrorMessage();

        private:
            //  The constructed arena object.
            Arena arena;
            //  The file stream to read from.
            std::ifstream* file;
            //  The result of the read operation.
            bool success = false;
            //  The internal method to parse the file and construct the arena.
            //  Returns true if successful, false otherwise.
            bool parseFile_();
            //  The error message if the read operation fails.
            std::string errmsg = "";
    };

}

#endif // CORE_ARENA_READER_HPP