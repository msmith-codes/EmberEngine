#include <Ember/utils/Logger.hpp>

// -- Custom -- //
#include <Ember/utils/Helper.hpp>

// -- STD -- //
#include <iostream>
#include <fstream>
#include <ctime>

namespace Ember
{
    // Static Variables
    //-------------------------------------------------------------------------
    bool Logger::logToFile = false;
    bool Logger::verbose = false;
    std::string Logger::logFileName;

    // Static Setup
    //-------------------------------------------------------------------------
    void Logger::init(bool logToFile, bool verbose)
    {
        Logger::logToFile = logToFile;
        Logger::verbose = verbose;

        Logger::logFileName = Logger::getFileName();
    }

    // Static Methods
    //-------------------------------------------------------------------------
    void Logger::debug(const std::string& message)
    {
        if(!Logger::verbose)  {
            return;
        }

        std::string time = Helper::getLocalTime();
        std::cout << "[" << time << "] [DEBUG] " << message << std::endl;
        if(Logger::logToFile) {
            std::ofstream file(Logger::logFileName, std::ios::app);
            file << "[" << time << "] [DEBUG] " << message << std::endl;
            file.close();
        }
    }

    void Logger::info(const std::string& message)
    {
        std::string time = Helper::getLocalTime();
        std::cout << "[" << time << "] [INFO] " << message << std::endl;
        if(Logger::logToFile) {
            std::ofstream file(Logger::logFileName, std::ios::app);
            file << "[" << time << "] [INFO] " << message << std::endl;
            file.close();
        }
    }

    void Logger::warn(const std::string& message)
    {
        std::string time = Helper::getLocalTime();
        std::cout << "[" << time << "] [WARN] " << message << std::endl;
        if(Logger::logToFile) {
            std::ofstream file(Logger::logFileName, std::ios::app);
            file << "[" << time << "] [WARN] " << message << std::endl;
            file.close();
        }
    }

    void Logger::error(const std::string& message)
    {
        std::string time = Helper::getLocalTime();
        std::cout << "[" << time << "] [ERROR] " << message << std::endl;
        if(Logger::logToFile) {
            std::ofstream file(Logger::logFileName, std::ios::app);
            file << "[" << time << "] [ERROR] " << message << std::endl;
            file.close();
        }
    }

    void Logger::fatal(const std::string& message)
    {
        std::string time = Helper::getLocalTime();
        std::cout << "[" << time << "] [FATAL] " << message << std::endl;
        if(Logger::logToFile) {
            std::ofstream file(Logger::logFileName, std::ios::app);
            file << "[" << time << "] [FATAL] " << message << std::endl;
            file.close();
        }
    }

    // Static Utilities
    //-------------------------------------------------------------------------
    std::string Logger::getFileName()
    {
        std::string time = Helper::getLocalTime();
        time.erase(std::remove(time.begin(), time.end(), ' '), time.end());
        time.erase(std::remove(time.begin(), time.end(), ':'), time.end());
        time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());
        return "log_" + time + ".log";
    }

}