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

    pattern_finder(const std::string text_file_path, const std::string pattern);
    virtual ~pattern_finder();

protected:
    // indeces at which the pattern was found
    std::vector<size_t> m_pattern_entries;
    
    const std::string m_text_file_path;
    // if we could not open the file, we'll treat its path as the text itself
    const std::string m_pattern;

    std::ifstream m_text_file;
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
        fs::remove(temp_file_name);
    }

    virtual void search() override;

private:
    size_t m_chars_count{ 0 };

    std::vector<size_t> prefix_func(std::string s);
    std::vector<size_t> get_pattern_entries(std::ifstream* s);

    std::string temp_file_name{"kmp_temp_file.txt"};
};

class boyer_moore_finder : public pattern_finder
{
public:
    boyer_moore_finder(const std::string text_file_path, const std::string pattern) :
        pattern_finder(text_file_path, pattern)
    {};

    virtual void search() override;
};

class rabin_karp_finder : public pattern_finder
{
public:
    rabin_karp_finder(const std::string text_file_path, const std::string pattern) :
        pattern_finder(text_file_path, pattern)
    {};

    virtual void search() override;

    int alph_length{ 256 };
    int prime_delim{ 101 };
};