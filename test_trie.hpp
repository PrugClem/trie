
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

#include "trie.hpp"

template<std::size_t children_count>
void simple_test(trie::basic_trie<children_count, std::string>& data, std::ostream &output_log)
{
    trie::basic_trie<children_count, std::string> second;
    data.insert(trie::basic_key<children_count>("ABC"), std::make_shared<std::string>("ABC"));
    data.insert(trie::basic_key<children_count>("DEF"), std::make_shared<std::string>("DEF"));
    second.insert(trie::basic_key<children_count>("DEF"), std::make_shared<std::string>("DEF"));
    data.insert(trie::basic_key<children_count>("GHI"), std::make_shared<std::string>("GHI"));
    data.insert(trie::basic_key<children_count>("JKL"), std::make_shared<std::string>("JKL"));
    second.insert(trie::basic_key<children_count>("MNO"), std::make_shared<std::string>("MNO"));
    data.insert(trie::basic_key<children_count>("PQR"), std::make_shared<std::string>("PQR"));
    second.insert(trie::basic_key<children_count>("STU"), std::make_shared<std::string>("STU"));
    data.insert(trie::basic_key<children_count>("VWX"), std::make_shared<std::string>("VWX"));
    second.insert(trie::basic_key<children_count>("YZ "), std::make_shared<std::string>("YZ "));

    output_log << std::endl << "Running simple trie test" << std::endl;

    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        output_log << "data.at(\"" << iter.get_key().to_string() << "\"): \"" << *iter.get_data() << "\"" << std::endl;
    }
    for (auto iter = second.begin(); iter != second.end(); iter++)
    {
        output_log << "second.at(\"" << iter.get_key().to_string() << "\"): \"" << *iter.get_data() << "\"" << std::endl;
    }

    output_log << "Running line: data.merge(second);" << std::endl;
    data.merge(second);

    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        output_log << "data.at(\"" << iter.get_key().to_string() << "\"): \"" << *iter.get_data() << "\"" << std::endl;
    }
    for (auto iter = second.begin(); iter != second.end(); iter++)
    {
        output_log << "second.at(\"" << iter.get_key().to_string() << "\"): \"" << *iter.get_data() << "\"" << std::endl;
    }

    output_log << "Running line: data.erase(\"DEF\"):" << std::endl
               << "Running line: data.merge(second);" << std::endl;
    data.erase(trie::basic_key<children_count>("DEF"));
    data.merge(second);

    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        output_log << "data.at(\"" << iter.get_key().to_string() << "\"): \"" << *iter.get_data() << "\"" << std::endl;
    }
    for (auto iter = second.begin(); iter != second.end(); iter++)
    {
        output_log << "second.at(\"" << iter.get_key().to_string() << "\"): \"" << *iter.get_data() << "\"" << std::endl;
        throw std::runtime_error("The second trie MUST be empty here!");
    }

    data.clear();
}

std::string limit_string(std::string input, std::size_t limit)
{
    return input.substr(0, std::min(input.length() - 1, limit));
    //return input;
}

template<std::size_t children_count>
void test_trie(trie::basic_trie<children_count, std::string>& data, std::ostream& output_log)
{
    std::size_t pairs = 0, nodes = 0;
    std::size_t counter_forward_iterator, counter_reverse_iterator;
    std::ifstream ifile("../../../test_data.txt");

    if (!ifile)
    {
        throw std::runtime_error("Error opening file");
    }

    read_test_file(ifile, data);
    output_log << std::endl;

    for (auto iter = data.node_begin(); iter != data.node_end(); iter++)
    {
        if (iter) nodes++;
        if (iter && iter.get_data()) pairs++;
        output_log << "counted " << nodes << " nodes containing " << pairs << " pairs at key [" << limit_string(iter.get_key().to_string(), 20) << "]             \r";
    }
    output_log << std::endl << "[forward node iterator] " << pairs << " key-value pairs stored into " << nodes << " trie nodes found in file" << std::endl;
    counter_forward_iterator = nodes;

    pairs = nodes = 0;
    for (auto iter = data.node_rbegin(); iter != data.node_rend(); iter++)
    {
        if (iter) nodes++;
        if (iter && iter.get_data()) pairs++;
        output_log << "counted " << nodes << " nodes containing " << pairs << " pairs at key [" << limit_string(iter.get_key().to_string(), 20) << "]             \r";
    }
    output_log << std::endl << "[reverse node iterator] " << pairs << " key-value pairs stored into " << nodes << " trie nodes found in file" << std::endl;
    counter_reverse_iterator = nodes;

    if (counter_forward_iterator != counter_reverse_iterator)
    {
        throw std::runtime_error("Error testing node iterators: number of nodes found does not match");
    }

    nodes = 0;
    for (auto iter = data.node_begin(); iter != data.node_end(); iter++)
    {
        std::cout << "Testing iterator reversability (" << (++nodes) << "/" << counter_forward_iterator << ")\r";
        auto copy = iter;
        copy++;
        copy--;
        if (iter != copy)
        {
            std::stringstream ss;
            ss << "Error testing iterator reversability\n"
                << "Error caused after key [" << iter.get_key().to_string() << "]";
            throw std::runtime_error(ss.str());
        }
    }

    pairs = 0;
    for (auto iter = data.begin(); iter != data.end(); iter++)
    {
        if (iter.is_null()) throw std::runtime_error("ERROR: ITERATOR IS NULL");
        if (iter.get_data() == nullptr) throw std::runtime_error("ERROR: NO DATA IN ITERATOR");
        output_log << "counted " << ++pairs << " pairs at key [" << limit_string(iter.get_key().to_string(), 20) << "]             \r";
    }
    output_log << std::endl << "[forward value iterator] " << pairs << " key-value pairs found in file" << std::endl;
    counter_forward_iterator = pairs;

    pairs = 0;
    for (auto iter = data.rbegin(); iter != data.rend(); iter++)
    {
        if (iter.is_null()) throw std::runtime_error("ERROR: ITERATOR IS NULL");
        if (iter.get_data() == nullptr) throw std::runtime_error("ERROR: NO DATA IN ITERATOR");
        output_log << "counted " << ++pairs << " pairs at key [" << limit_string(iter.get_key().to_string(), 20) << "]             \r";
    }
    output_log << std::endl << "[reverse value iterator] " << pairs << " key-value pairs found in file" << std::endl;
    counter_reverse_iterator = pairs;

    if (counter_forward_iterator != counter_reverse_iterator)
    {
        throw std::runtime_error("Error testing value iterators: number of pairs found does not match");
    }

    data.clear();
}
