/**
* @file     trie.cpp
* @brief    main source file for esting purposes
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

#if 0
int main()
{
    trie::trie16_t<std::string> test;

    test.insert(trie::key16_t("ABC"), std::make_shared<std::string>("ABC"));
}
#else

template<typename value_t> using trie_t = trie::trie16_t<value_t>;

void read_test_file(std::istream& input, trie_t<std::string>& output)
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
}

int main()
{
    std::ifstream ifile("test_data.txt");
    if (!ifile)
    {
        std::cerr << "unable to open data file!" << std::endl;
        return 1;
    }

    std::size_t pairs = 0, nodes = 0;
    trie_t<std::string> file_content;
    read_test_file(ifile, file_content);
    std::cout << std::endl;
    for (auto iter = file_content.node_begin(); iter != file_content.node_end(); iter++)
    {
        if (iter) nodes++;
        if (iter && iter.get_data()) pairs++;
        std::cout << "counted " << nodes << " nodes containing " << pairs << " pairs at key " << limit_string(iter.get_key().to_hex_string(), 20) << "             \r";
    }
    std::cout << std::endl << pairs << " key-value pairs stored into " << nodes << " trie nodes found in file" << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}
#endif
