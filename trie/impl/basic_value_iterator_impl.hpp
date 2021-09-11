/**
* @file     trie/impl/basic_value_iterator_impl.hpp
* @brief    include file for the implementations for base value iterator class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_trie.hpp"

template<std::size_t children_count, typename value_t>
void trie::basic_trie<children_count, value_t>::basic_value_iterator::next_value() const
{
    do
    {
        this->next_node();
    }
    /*
    * continue until either:
    *  1. node is the null node (*this using bool typecast)
    *  2. node contains a value (this->get_data() == nullptr)
    */
    while ((!this->is_null()) && (this->get_data() == nullptr));
}

template<std::size_t children_count, typename value_t>
void trie::basic_trie<children_count, value_t>::basic_value_iterator::prev_value() const
{
    do
    {
        this->prev_node();
    }
    /*
    * continue until either:
    *  1. node is the null node (*this using bool typecast)
    *  2. node contains a value (this->get_data() == nullptr)
    */
    while ((!this->is_null()) && (this->get_data() == nullptr));
}
