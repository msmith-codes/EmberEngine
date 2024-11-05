#ifndef EMBER_Logger_HPP_
#define EMBER_Logger_HPP_

// -- STD -- //
#include <string>

namespace Ember
{
    class Logger
    {
        private: // Static Variables
            static bool logToFile;
            static bool verbose;
            static std::string logFileName;
        public: // Static Setup
            static void init(bool logToFile, bool verbose);
        public: // Static Methods
            static void debug(const std::string& message);
            static void info(const std::string& message);
            static void warn(const std::string& message);
            static void error(const std::string& message);
            static void fatal(const std::string& message);
        private: // Static Utilities
            static std::string getFileName();

    };
}

#endif