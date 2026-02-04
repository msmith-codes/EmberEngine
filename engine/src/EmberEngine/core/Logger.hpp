#pragma once

#include <string>
#include <vector>

namespace EmberEngine
{
    class Logger
    {
        private:
            static std::vector<std::string> log_cache;
        public:
            static void info(const std::string& system, const std::string& message);
            static void warn(const std::string& system, const std::string& message);
            static void error(const std::string& system, const std::string& message);
            static void dump_to_file();
        private:
            static void log(const std::string& prefix, const std::string& system, const std::string& message);
            static std::string get_time();
    };
}