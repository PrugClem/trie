/**
* @file     trie.cpp
* @brief    main source file for testing purposes
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <iostream>
#include <fstream>

#include "trie.hpp"

template<typename key_t>
void read_test_file(std::istream& input, trie::basic_trie<key_t, std::string>& output)
{
    std::string stringbuffer, key, value;
    std::size_t index, length, line_number = 0, pair_count = 0;
    bool valid_line;
    // read everything from the istream
    while(!input.eof())
    {
        // get one line at the time
        // the lines can be taken from a *.json file, with the a format like `*"key"*:*"value*"*`
        std::getline(input, stringbuffer);
        std::cout << "reading file line #" << ++line_number << "\r";
        valid_line = false;

        // read key string
        index = stringbuffer.find('\"');
        if (index < stringbuffer.size())
        {
            stringbuffer = stringbuffer.substr(index + 1, stringbuffer.size() - 1);
            length = stringbuffer.find('\"');
            if (length < stringbuffer.size())
            {
                key = stringbuffer.substr(0, length);
                stringbuffer = stringbuffer.substr(length + 1, stringbuffer.size() - 1);

                // read value string
                index = stringbuffer.find('\"');
                if (index < stringbuffer.size())
                {
                    stringbuffer = stringbuffer.substr(index + 1, stringbuffer.size() - 1);
                    length = stringbuffer.find('\"');
                    if (length < stringbuffer.size())
                    {
                        value = stringbuffer.substr(0, length);

                        output.insert(key, std::make_shared<std::string>(value));
                        pair_count++;
                    }
                }
            }
        }
    }
    std::cout << "\nread " << pair_count << " pairs from input file" << std::endl;
}

std::string limit_string(std::string input, std::size_t limit)
{
    return input.substr(0, std::min(input.length() - 1, limit));
    //return input;
}

template<typename key_t>
void test_trie(trie::basic_trie<key_t, std::string>& data, std::ostream& output_log)
{
    std::size_t pairs = 0, nodes = 0;
    std::ifstream ifile("../../../test_data.txt");

    if (!ifile)
    {
        throw std::runtime_error("Error opening file");
    }

    read_test_file(ifile, data);
    output_log << std::endl;

    for(auto iter = data.node_begin(); iter != data.node_end(); iter++)
    {
        if (iter) nodes++;
        if (iter && iter.get_data()) pairs++;
        output_log << "counted " << nodes << " nodes containing " << pairs << " pairs at key " << limit_string(iter.get_key().to_hex_string(), 20) << "             \r";
    }
    output_log << std::endl << "[forward node iterator] " << pairs << " key-value pairs stored into " << nodes << " trie nodes found in file" << std::endl;

    pairs = nodes = 0;
    for (auto iter = data.node_rbegin(); iter != data.node_rend(); iter++)
    {
        if (iter) nodes++;
        if (iter && iter.get_data()) pairs++;
        output_log << "counted " << nodes << " nodes containing " << pairs << " pairs at key " << limit_string(iter.get_key().to_hex_string(), 20) << "             \r";
    }
    output_log << std::endl << "[reverse node iterator] " << pairs << " key-value pairs stored into " << nodes << " trie nodes found in file" << std::endl;

    pairs = 0;
    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        if (iter.is_null()) throw std::runtime_error("ERROR: ITERATOR IS NULL");
        if (iter.get_data() == nullptr) throw std::runtime_error("ERROR: NO DATA IN ITERATOR");
        output_log << "counted " << ++pairs << " pairs at key " << limit_string(iter.get_key().to_hex_string(), 20) << "             \r";
    }
    output_log << std::endl << "[forward value iterator] " << pairs << " key-value pairs found in file" << std::endl;

    pairs = 0;
    for(auto iter = data.rbegin(); iter != data.rend(); iter++)
    {
        if (iter.is_null()) throw std::runtime_error("ERROR: ITERATOR IS NULL");
        if (iter.get_data() == nullptr) throw std::runtime_error("ERROR: NO DATA IN ITERATOR");
        output_log << "counted " << ++pairs << " pairs at key " << limit_string(iter.get_key().to_hex_string(), 20) << "             \r";
    }
    output_log << std::endl << "[reverse value iterator] " << pairs << " key-value pairs found in file" << std::endl;
}

int main()
{
    std::ofstream logfile("latest.log", std::ios::binary);
    if (!logfile) throw std::runtime_error("Error opening logfile");
    trie::trie256_t<std::string> trie256;
    trie::trie16_t<std::string> trie16;

    std::cout << std::endl << "Testing 256-children trie" << std::endl
        << "================================" << std::endl;
    test_trie(trie256, std::cout);
    std::cout << std::endl << "Testing 16-children trie" << std::endl
        << "================================" << std::endl;
    test_trie(trie16, std::cout);

    return 0;
}
