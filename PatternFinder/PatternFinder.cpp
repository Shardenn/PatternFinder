#include "PatternFinder.h"
#include "logger.hpp"
#include <assert.h>
#include <math.h>

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
{}

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
            res.push_back(current_index - m_pattern.size() + 2);
        current_index++;
    }
    return res;
}

boyer_moore_finder::boyer_moore_finder(const std::string text_file_path, const std::string pattern) :
    pattern_finder(text_file_path, pattern)
{

}

void boyer_moore_finder::search()
{

}

rabin_karp_finder::rabin_karp_finder(const std::string text_file_path, const std::string pattern) :
    pattern_finder(text_file_path, pattern)
{

}

void rabin_karp_finder::search()
{
    int pattern_len = m_pattern.length();
    
    int i, j;
    
    int p = 0; // hash value for pattern 
    int t = 0; // hash value for txt 
    int h = 1;

    // The value of h would be "pow(d, M-1)%q" 
    h = static_cast<int>(pow(static_cast<double>(alph_length), static_cast<double>(pattern_len - 1))) % prime_delim;

    // Calculate the hash value of pattern and first 
    // window of text 
    for (i = 0; i < pattern_len; i++)
    {
        p = (alph_length * p + m_pattern[i]) % prime_delim;
        t = (alph_length * t + m_text_file.get()) % prime_delim;
    }

    // Slide the pattern over text one by one 
    m_text_file.seekg(0);
    char *symbols = new char[pattern_len + 1];
    while(m_text_file.peek() != EOF)
    {
        // Check the hash values of current window of text 
        // and pattern. If the hash values match then only 
        // check for characters on by one         
        if (p == t)
        {
            auto previous_position = m_text_file.tellg();
            /* Check for characters one by one */
            for (j = 0; j < pattern_len && m_text_file.peek() != EOF; j++)
            {
                auto ch = m_text_file.get();
                if (ch != m_pattern[j])
                    break;
            }

            // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1] 
            if (j == pattern_len)
            {
                int position_found = previous_position;
                m_pattern_entries.push_back(position_found + 1);
            }

            m_text_file.seekg(previous_position);
        }

        //auto ch = m_text_file.get();
        // Calculate hash value for next window of text: Remove 
        // leading digit, add trailing digit 
        if(m_text_file.peek() != EOF)
        {
            auto previous_position = m_text_file.tellg();

            char lost_symbol = m_text_file.get();

            
            m_text_file.read(symbols, pattern_len + 1);
            char new_symbol = symbols[pattern_len - 1];

            // casting because char == unsigned int and it results into wrong arithmetics
            t = ((alph_length * (t - static_cast<int>(lost_symbol) * h)) + static_cast<int>(new_symbol)) % prime_delim;

            // We might get negative value of t, converting it 
            // to positive 
            if (t < 0)
                t = (t + prime_delim);

            
            m_text_file.seekg(previous_position);
            if (!m_text_file.get())
                break;
        }
    }
    delete[] symbols;
}
