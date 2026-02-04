#include <EmberEngine/core/Logger.hpp>

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

namespace EmberEngine
{
    std::vector<std::string> Logger::log_cache;

    void Logger::info(const std::string& system, const std::string& message)
    {
        Logger::log("INFO", system, message);
    }

    void Logger::warn(const std::string& system, const std::string& message)
    {
        Logger::log("WARN", system, message);
    }

    void Logger::error(const std::string& system, const std::string& message)
    {
        Logger::log("ERROR", system, message);
    }

    void Logger::dump_to_file()
    {
        std::string filename = "log_" + Logger::get_time();
        for (char& c : filename) {
            if (c == ':' || c == '.') {
                c = '_';
            }
        }
        filename += ".txt";
        
        std::ofstream log_file(filename);
        if (log_file.is_open()) {
            for (const auto& log_entry : log_cache) {
                log_file << log_entry << std::endl;
            }
            log_file.close();
        }
    }

    void Logger::log(const std::string& prefix, const std::string& system, const std::string& message)
    {
        std::string formatted_time = Logger::get_time();
        std::string log_entry = "[" + prefix + "/" + system + "] [" + formatted_time + "] : " + message;
        log_cache.push_back(log_entry);
        std::cout << log_entry << std::endl;
    }

    std::string Logger::get_time()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::tm tm_time;
        localtime_s(&tm_time, &time);
        
        std::ostringstream oss;
        oss << std::put_time(&tm_time, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
    }
}