#include <iostream>
#include <chrono>
#include "PatternFinder.h"
#include "logger.hpp"

enum finder_type_t
{
    naive,
    kmp,
    bm,
    rk
};

int main(int argc, char* argv[])
{
    std::string file_path;
    std::string pattern;
    finder_type_t finder_type;
        
    bool args_ok = true;
    for (int i = 1; i < argc; i++)
    {
        std::string token{ argv[i] };

        if (token == "-h" || token == "--help" || token == "--h" || token == "-help")
        {
            std::cout << "Program usage:\n"
                "[-h | --help | --h | -help] prints possible input arguments\n"
                "[--naive | --kmp | --bm | --rk] selects one of the algorithms\n"
                "[--file <file_path>] specifies the input file path\n"
                "[--pattern <pattern>] specifies the pattern to search for\n";
            return 0;
        }
        else if (token == "--file")
        {
            file_path = argv[++i];
        }
        else if (token == "--pattern")
        {
            pattern = argv[++i];
        }
        else if (token == "--naive")
        {
            finder_type = naive;
        }
        else if (token == "--kmp")
        {
            finder_type = kmp;
        }
        else if (token == "--bm")
        {
            finder_type = bm;
        }
        else if (token == "--rk")
        {
            finder_type = rk;
        }
        else
        {
            Logger::log("Unknown argument: " + token, verbosity_t::error);
            args_ok = false;
        }
    }

    if (!args_ok)
    {
        Logger::log("Check your arguments spelling. Terminating...");
        return 0;
    }

    std::unique_ptr<pattern_finder> finder;
    switch (finder_type)
    {
        case naive:
            finder = std::unique_ptr<naive_finder>{ new naive_finder(file_path, pattern) };
            break;
        case kmp:
            finder = std::unique_ptr<kmp_finder>{ new kmp_finder(file_path, pattern) };
            break;
        case bm:
            finder = std::unique_ptr<boyer_moore_finder>{ new boyer_moore_finder(file_path, pattern) };
            break;
        case rk:
            finder = std::unique_ptr<rabin_karp_finder>{ new rabin_karp_finder(file_path, pattern) };
            break;
    }
    
    Logger logger;
    
    logger.remember_time();
    finder->search();
    logger.print_duration();

    finder->print_result();
    
    return 0;
}