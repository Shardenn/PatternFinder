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

kmp_finder::kmp_finder(const std::string text_file_path, const std::string pattern) :
    pattern_finder(text_file_path, pattern)
{
    namespace fs = std::filesystem;

    fs::remove(temp_file_name);
    // create new file
    std::ofstream temp_file_stream(temp_file_name, std::ios::trunc);

    assert(temp_file_stream.is_open());

    temp_file_stream << pattern;
    temp_file_stream << "|";
    
    char* c = new char;
    std::ifstream orig_file(text_file_path, std::ios::in);
    while (orig_file.peek() != EOF)
    {
        orig_file.read(c, 1);
        temp_file_stream.write(c, 1);
    }
    delete c;
}

void kmp_finder::search()
{

}

std::vector<size_t> kmp_finder::prefix_func(std::string s)
{
    size_t n = s.length();

    std::vector<size_t> p(n);
    p[0] = 0;

    for (size_t i = 1; i < n; i++)
    {
        size_t j = p[i - 1];
        while ((j > 0) && (s[i] != s[j]))
            j = p[j - 1];
        if (s[j] == s[i])
            j++;

        p[i] = j;
    }
    return p;
}