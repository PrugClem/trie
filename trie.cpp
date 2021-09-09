// trie.cpp : Defines the entry point for the application.
//

#include "trie.hpp"

#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>

constexpr std::size_t iterations = 10'000;

void read_test_file(std::istream& input, trie::trie<std::string>& output)
{
    std::string stringbuffer, key, value;
    std::size_t index, length, line_number = 0;
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
                    }
                }
            }
        }
    }
}

std::string limit_string(std::string input, std::size_t limit)
{
    return input.substr(0, std::min(input.length() - 1, limit));
}

int main()
{
    std::chrono::system_clock::time_point t0, start;

#if 1
    std::ifstream ifile("../../../test_data.txt");
    if (!ifile)
    {
        std::cerr << "unable to open data file!" << std::endl;
        return 1;
    }

    std::size_t pairs = 0, nodes = 0;
    trie::trie<std::string> file_content;
    read_test_file(ifile, file_content);
    std::cout << std::endl;
    for (auto iter = file_content.node_begin(); iter != file_content.node_end(); iter++)
    {
        if (iter) nodes++;
        if (iter && iter.get_data()) pairs++;
        std::cout << "counted " << nodes << " nodes containing " << pairs << " pairs at key " << limit_string(iter.get_key().to_hex_string(), 20) << "             \r";
    }
    std::cout << std::endl << pairs << " key-value pairs stored into " << nodes << " trie nodes totaling " << (nodes * ::trie::trie<std::string>::node_size ) << " bytes found in file" << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
#else
    {
        trie::trie<std::string> test, sub, copy;

        test.insert(trie::key("TEST"), std::make_shared<std::string>("TEST"));
        test.insert(trie::key("ABCDEF"), std::make_shared<std::string>("ABCDEF"));
        test.insert(trie::key("ABC"), std::make_shared<std::string>("ABC"));
        test.insert(trie::key("ABCEDF"), nullptr);
        test.insert(trie::key("Z"), nullptr);

        std::cout << "trie.at(\"ABC\"): " << test.at(trie::key("ABC")) << std::endl;

        std::cout << "Printing main trie, regular order" << std::endl;
        for (auto iter = test.node_begin(); iter != test.node_end(); iter++)
        {
            std::cout << "test.at(\"" << iter.get_key().to_string() << "\"): " << iter.get_data() << std::endl;
        }

        sub = test.subtrie(trie::key("ABC"));
        sub.insert(trie::key("-TEST"), std::make_shared<std::string>("ABC-TEST"));

        std::cout << "Printing subtrie, regular order" << std::endl;
        for (auto iter = sub.node_begin(); iter != sub.node_end(); iter++)
        {
            std::cout << "sub.at(\"" << iter.get_key().to_string() << "\"): " << iter.get_data() << std::endl;
        }
        std::cout << "Printing subtrie, reverse order" << std::endl;
        for (auto riter = sub.node_rbegin(); riter != sub.node_rend(); riter++)
        {
            std::cout << "sub.at(\"" << riter.get_key().to_string() << "\"): " << riter.get_data() << std::endl;
        }

        copy = sub.clone();
        copy.insert(trie::key("-CBA"), std::make_shared < std::string>("ABC-CBA"));

        std::cout << "Printing cloned and modified subtrie, regular order" << std::endl;
        for (auto iter = copy.node_begin(); iter != copy.node_end(); iter++)
        {
            std::cout << "copy.at(\"" << iter.get_key().to_string() << "\"): " << iter.get_data() << std::endl;
        }
    }

    t0 = std::chrono::system_clock::now();
    for (size_t i = 0; i < iterations; i++)
    {
        start = std::chrono::system_clock::now();
        trie::trie<std::string> data;

        data.insert(trie::key("TEST"), std::make_shared<std::string>("TEST"));
        data.insert(trie::key("ABCDEF"), nullptr);
        data.insert(trie::key("ABC"), std::make_shared<std::string>("ABC"));
        data.insert(trie::key("ABCEDF"), nullptr);

        if (*data.at(trie::key("ABC")) != "ABC") { std::cerr << "Error getting the data"; }

        //data.print_trie(std::cout);
        std::cout << "Iteration #" << i << " done in " << ((std::chrono::system_clock::now() - start) / std::chrono::microseconds(1)) << "us      \r";
    }
    std::cout << iterations << " Iterations done in " << ((std::chrono::system_clock::now() - t0) / std::chrono::milliseconds(1)) << "ms      " << std::endl;
    return 0;
#endif
}
