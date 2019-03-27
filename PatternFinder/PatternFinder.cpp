#include "PatternFinder.h"
#include "logger.hpp"
#include <assert.h>


pattern_finder::pattern_finder(const std::string text_file_path, const std::string pattern) :
    m_pattern(pattern)
{
    m_text_file.open(text_file_path, std::ios::in | std::ios::binary);

    assert(m_text_file.is_open());
    assert(pattern != "");

    Logger::log("File " + text_file_path + " opened successfully", verbosity_t::info);
    //b_init_ok = true;
}

void pattern_finder::print_result()
{
    Logger::log("Found " + std::to_string(m_pattern_entries.size()) + " entries: ");
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
    Logger::log("Naive finder started...");

    size_t len_pattern = m_pattern.length();

    //previous_position = 0;
    int file_pos = 0;
    char c;
    while (!m_text_file.eof() && m_text_file.peek() != EOF)
    {
        int pat_index = 0;
        while (pat_index < len_pattern && !m_text_file.eof())
        {
            m_text_file.get(c);
            file_pos++;

            if (c != m_pattern[pat_index])
                break;
            pat_index++;

            if (pat_index == len_pattern)
            {
                m_pattern_entries.push_back(file_pos + 1 - len_pattern);
            }
        }


        //m_text_file.seekg(file_pos);
    }

    Logger::log("Naive finder finished.");
}

kmp_finder::kmp_finder(const std::string text_file_path, const std::string pattern) :
    pattern_finder(text_file_path, pattern)
{
    // create new file
    std::ofstream temp_file_stream(temp_file_name, std::ios::trunc | std::ios::binary);

    assert(temp_file_stream.is_open());

    temp_file_stream << pattern;
    temp_file_stream << "|";

    char* c = new char;
    std::ifstream orig_file(text_file_path, std::ios::in | std::ios::binary);
    while (orig_file.peek() != EOF)
    {
        orig_file.read(c, 1);
        temp_file_stream.write(c, 1);
    }
    delete c;

    temp_file_stream.close();

    // open the file again in read mode to get characters count
    std::ifstream temp_file(temp_file_name, std::ios::in | std::ios::binary);

    char character;
    while (!temp_file.eof())
    {
        temp_file.get(character);
        m_chars_count++;
    }
    temp_file.close();
}

void kmp_finder::search()
{
    Logger::log("KMP finder started...");

    m_pattern_entries = get_pattern_entries(&m_text_file);

    Logger::log("KMP finder finished.");
}

std::vector<size_t> kmp_finder::prefix_func(std::string s)
{
    size_t n = s.length();

    assert(n != 0);

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

std::vector<size_t> kmp_finder::get_pattern_entries(std::ifstream* s)
{
    assert(m_chars_count != 0);

    std::vector<size_t> pi = prefix_func(m_pattern);

    std::vector<size_t> res;
    int current_index = 0, j = 0;
    char text_char;
    while(s->get(text_char))
    {
        while (j > 0 && text_char != m_pattern[j])
            j = pi[j - 1];
        if (text_char == m_pattern[j])
            j++;
        if (j == m_pattern.size())
            res.push_back(current_index - m_pattern.size() + 1);
        current_index++;
    }
    return res;
}
