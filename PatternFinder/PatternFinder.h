#pragma once

#include <vector>
#include <fstream>
#include <filesystem>
#include <string>

class pattern_finder
{
public:
    virtual void search() {};
    void print_result();
    //bool is_initialized_successfully() { return b_init_ok; }

    pattern_finder(const std::string text_file_path, const std::string pattern);
    virtual ~pattern_finder();

protected:
    // indeces at which the pattern was found
    std::vector<size_t> m_pattern_entries;
    
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

class kmp_finder : public pattern_finder
{
public:
    kmp_finder(const std::string text_file_path, const std::string pattern);

    virtual ~kmp_finder()
    {
        namespace fs = std::filesystem;
        auto workdir = fs::current_path();
        fs::remove(workdir / temp_file_name);
    }

    virtual void search() override;

//private:
    std::vector<size_t> prefix_func(std::string s);

    std::string temp_file_name{"kmp_temp_file.txt"};
};