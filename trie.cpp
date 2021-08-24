// trie.cpp : Defines the entry point for the application.
//

#include "trie.hpp"

#include <iostream>
#include <chrono>

constexpr std::size_t iterations = 10'000;

int main()
{
    std::chrono::system_clock::time_point t0, start;


    {
        trie::trie<std::string> test, sub;

        test.insert(trie::key("TEST"), std::make_shared<std::string>("TEST"));
        test.insert(trie::key("ABCDEF"), std::make_shared<std::string>("ABCDEF"));
        test.insert(trie::key("ABC"), std::make_shared<std::string>("ABC"));
        test.insert(trie::key("ABCEDF"), nullptr);
        test.insert(trie::key("Z"), nullptr);

        //std::cout << "test.at(\"ABC\"): " << *test.at(trie::key("ABC")) << std::endl;
        test.print_trie(std::cout);

        std::cout << "begin: [" << test.node_begin().get_key() << "] : " << test.node_begin().get_data() << std::endl;
        //std::cout << "end:   [" << test.node_end()  .get_key() << "] : " << test.node_end()  .get_data() << std::endl;
        for(trie::trie<std::string>::node_iterator iter = test.node_begin(); iter != test.node_end(); iter++)
        {
            std::cout << "[" << iter.get_key() << "] : " << iter.get_data() << std::endl;
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
}
