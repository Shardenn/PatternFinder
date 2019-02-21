#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

enum verbosity_t
{
    info,
    warning,
    error
};

class Logger
{
public:
    Logger() {};
    static void log(std::string text, verbosity_t verbosity = verbosity_t::info, bool log_time = false)
    {
        if (log_time)
        {
            auto a = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(a);
            std::cout << ctime(&time) << " ";
        }
        switch (verbosity)
        {
            case info:
                std::cout << "INFO: ";
                break;
            case warning:
                std::cout << "WARN: ";
                break;
            case error:
                std::cout << "ERROR: ";
                break;
        }
        std::cout << text << std::endl;
    }
private:
    // i love cpp btw
    std::chrono::time_point<std::chrono::system_clock> m_remembered_time = std::chrono::system_clock::now();
};