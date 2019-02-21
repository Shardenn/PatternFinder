#include <iostream>
#include <chrono>
#include "PatternFinder.h"

int main(int argc, char* argv[])
{
    naive_finder finder("input.txt", "pattern");
    finder.search();
    finder.print_result();
    return 0;
}