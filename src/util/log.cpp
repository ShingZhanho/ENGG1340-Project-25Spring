#include <util/log.hpp>

#include <fstream>

#include <ctime>
#include <iomanip>
#include <sstream>

namespace util {
    void WriteToLog(const std::string& message, const std::string& caller, const std::string& tag) {
        const std::string logFile = "./game.log";
        std::ofstream logStream(logFile.c_str(), std::ofstream::app);
        if (!logStream.is_open()) {
            // Try to create the file if it doesn't exist
            logStream.open(logFile.c_str(), std::ofstream::out | std::ofstream::trunc);
            logStream << std::endl;
            logStream.close();
        }

        // Get current time
        std::time_t now = std::time(nullptr);
        std::tm* localtm = std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(localtm, "%Y-%m-%d %H:%M:%S");
        std::string timeStr = oss.str();

        logStream << timeStr << " [" << tag << "]";
        if (!caller.empty()) {
            logStream << " (" << caller << ")";
        }
        logStream << " " << message << std::endl;
        logStream.close();
    }
}