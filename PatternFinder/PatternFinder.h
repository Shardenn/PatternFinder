#pragma once

#include <vector>
#include <fstream>

class pattern_finder
{
public:
    virtual void search() = 0;
    void print_result();
    //bool is_initialized_successfully() { return b_init_ok; }

    pattern_finder(const std::string text_file_path, const std::string pattern);
    ~pattern_finder();

protected:
    // indeces at which the pattern was found
    std::vector<int> m_pattern_entries;
    
    const std::string m_text_file_path;
    // if we could not open the file, we'll treat its path as the text itself
    //const std::string m_text; 
    const std::string m_pattern;

    std::ifstream m_text_file;

    //bool b_init_ok;
};

class naive_finder : public pattern_finder
{
public:
    naive_finder(const std::string text_file_path, const std::string pattern) :
        pattern_finder(text_file_path, pattern)
    {};

    virtual void search() override;
};