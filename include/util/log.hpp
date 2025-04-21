#ifndef UTIL_LOG_HPP
#define UTIL_LOG_HPP

#include <string>

namespace util {

    //  Writes a message to the log file.
    //  Format: YYYY-MM-DD HH:MM:SS [TAG] (CALLER) MESSAGE
    void WriteToLog(const std::string& message, const std::string& caller = "?", const std::string& tag = "INFO");

}
#endif // UTIL_LOG_HPP