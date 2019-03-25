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

using Clock = std::chrono::high_resolution_clock;

class Logger
{
public:
    Logger() {};
    static void log(std::string text, verbosity_t verbosity = verbosity_t::info)
    {
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

    void remember_time()
    {
        m_remembered_time = Clock::now();
    }

    void print_duration()
    {
        auto now = Clock::now();

        using ms = std::chrono::milliseconds;
        auto duration = std::chrono::duration_cast<ms>(now - m_remembered_time).count();
        std::string duration_str = std::to_string(duration);
        log("Time since last remembered time is: " + duration_str);
    }

    // cant type "auto" here
    std::chrono::time_point<Clock> m_remembered_time = Clock::now();
};