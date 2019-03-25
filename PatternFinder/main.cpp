#include <iostream>
#include <chrono>
#include "PatternFinder.h"
#include "logger.hpp"

enum finder_type_t
{
    naive,
    kmp,
    bm
};

int main(int argc, char* argv[])
{
    std::string file_path;
    std::string pattern;
    finder_type_t finder_type;

    pattern_finder* finder{ nullptr };

    for (int i = 1; i < argc; i++)
    {
        std::string token{ argv[i] };

        if (token == "-h" || token == "--help" || token == "--h" || token == "-help")
        {
            std::cout << "Program usage:\n"
                "[-h | --help | --h | -help] prints possible input arguments\n"
                "[--naive | --kmp | --bm] selects one of the algorithms\n"
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
    }

    switch (finder_type)
    {
        case naive:
            finder = new naive_finder(file_path, pattern);
            break;
        case kmp:
            finder = new kmp_finder(file_path, pattern);
            break;
    }
    
    Logger logger;

    logger.remember_time();
    finder->search();
    logger.print_duration();

    finder->print_result();

    return 0;
}