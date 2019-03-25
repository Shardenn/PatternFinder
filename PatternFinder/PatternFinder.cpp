#include "PatternFinder.h"
#include "logger.hpp"

#include <assert.h>

pattern_finder::pattern_finder(const std::string text_file_path, const std::string pattern) :
    m_pattern(pattern)
{
    m_text_file.open(text_file_path, std::ios::in);
    
    assert(m_text_file.is_open());
    assert(pattern != "");

    Logger::log("File " + text_file_path + " opened successfully", verbosity_t::info);
    //b_init_ok = true;
}

void pattern_finder::print_result()
{
    Logger::log("Found entries:");
    for (auto el : m_pattern_entries)
    {
        Logger::log(std::to_string(el));
    }
}

pattern_finder::~pattern_finder()
{
    m_text_file.close();
}

void naive_finder::search()
{
    if (!m_text_file.is_open())
    {
        m_text_file.open(m_text_file_path, std::ios::in);
        if (!m_text_file.is_open())
        {
            Logger::log("File " + m_text_file_path + " could not be opened", verbosity_t::error);
            return;
        }
    }

    size_t len_pattern = m_pattern.length();

    //previous_position = 0;
    int file_pos = 0;
    char* ch = new char;
    while (!m_text_file.eof() && m_text_file.peek() != EOF)
    {
        int pat_index = 0;
        while (pat_index <= m_pattern.size() && !m_text_file.eof())
        {
            m_text_file.read(ch, 1);
            if (*ch != m_pattern[pat_index])
                break;
            pat_index++;

            if (pat_index + 1 == m_pattern.size())
            {
                //Logger::log("Found match at position " + std::to_string(file_pos));
                m_pattern_entries.push_back(file_pos);
            }
        }
        file_pos++;
        
        m_text_file.seekg(file_pos);
    }

    delete ch;
}

void kmp_finder::search()
{

}