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
#include <sstream>
#include <thread>

#include "trie.hpp"
#include "test_trie.hpp"

template<std::size_t children_count>
void read_test_file(std::istream& input, trie::basic_trie<children_count, std::string>& output)
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

int main()
{
    //std::ofstream logfile("latest.log", std::ios::binary);
    //if (!logfile) throw std::runtime_error("Error opening logfile");
    trie::trie256<std::string> trie256;
    trie::trie16<std::string> trie16;
    trie::trie4<std::string> trie4;
    trie::trie2<std::string> trie2;

    std::ostream& output = std::cout;
    
    std::cout << std::endl << "Testing 256-children trie (simple test)" << std::endl
        << "================================" << std::endl;
    simple_test(trie256, output);

    std::cout << std::endl << "Testing 16-children trie (simple test)" << std::endl
        << "================================" << std::endl;
    simple_test(trie16, output);

    std::cout << std::endl << "Testing 4-children trie (simple test)" << std::endl
        << "================================" << std::endl;
    simple_test(trie4, output);

    std::cout << std::endl << "Testing 2-children trie (simple test)" << std::endl
        << "================================" << std::endl;
    simple_test(trie2, output);



    std::cout << std::endl << "Testing 256-children trie" << std::endl
        << "================================" << std::endl;
    test_trie(trie256, output);

    std::cout << std::endl << "Testing 16-children trie" << std::endl
        << "================================" << std::endl;
    test_trie(trie16, output);

    std::cout << std::endl << "Testing 4-children trie" << std::endl
        << "================================" << std::endl;
    test_trie(trie4, output);

    std::cout << std::endl << "Testing 2-children trie" << std::endl
        << "================================" << std::endl;
    test_trie(trie2, output);

    return 0;
}
