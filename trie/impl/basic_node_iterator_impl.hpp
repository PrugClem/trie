/**
* @file     trie/impl/basic_node_iterator_impl.hpp
* @brief    include file for the implementations for base node iterator class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_trie.hpp"

template<typename key_t, typename value_t>
bool trie::basic_trie<key_t, value_t>::basic_node_iterator::operator==(const trie::basic_trie<key_t, value_t>::basic_node_iterator& other) const
{
    if (this->root_node != other.root_node) throw std::out_of_range("Iterators are not obtained from the same trie!");
    return this->cur_node == other.cur_node;
}

template<typename key_t, typename value_t>
bool trie::basic_trie<key_t, value_t>::basic_node_iterator::operator!=(const trie::basic_trie<key_t, value_t>::basic_node_iterator& other) const
{
    return !(*this == other);
}

template<typename key_t, typename value_t>
void trie::basic_trie<key_t, value_t>::basic_node_iterator::next_node() const
{
    while (true) // the loop is neccessary for the code to function properly, it will return eventually unless the trie is malformed
    {
        // if the current node is the null node, go to the root node
        if (cur_node == nullptr)
        {
            cur_node = root_node;
            cur_key.clear();
            child_element = 0;
            return;
        }
        // find the next child
        for (std::size_t i = child_element; i < key_t::children_count; i++)
        {
            if (cur_node->get_child(uint8_t(i)) != nullptr)
            {
                // if a child was found, go to child node and stop iterating
                cur_node = cur_node->get_child(uint8_t(i));
                cur_key.push_back(uint8_t(i));
                child_element = 0;
                return;
            }
        }
        // no child found, go to parent ( which is the null ode for the root node )
        if (cur_node == root_node)
        {
            cur_node = nullptr;
            cur_key.clear();
            child_element = 0;
            return;
        }
        child_element = (std::size_t)cur_key.get_element(cur_key.size() - 1) + 1;
        cur_key.pop_back();
        cur_node = trie::basic_trie<key_t, value_t>(root_node).get_node(cur_key);
    }
}

template<typename key_t, typename value_t>
void trie::basic_trie<key_t, value_t>::basic_node_iterator::prev_node() const
{
    if (cur_node == nullptr)
    {
        cur_node = root_node;
        cur_key.clear();
        child_key = children_count - 1;
    }
    else if (cur_node == root_node)
    {
        cur_node = nullptr;
        cur_key.clear();
        child_key = 0;
        return;
    }
    else
    {
        child_key = (std::size_t)cur_key.get_element(cur_key.size() - 1) - 1;
        cur_key.pop_back();
        cur_node = ::trie::trie<T>(root_node).get_node(cur_key);
    }

    while (true)
    {
        for (std::ptrdiff_t i = child_key; i >= 0; i--)
        {
            if (cur_node->get_child((uint8_t)i) != nullptr)
            {
                cur_node = cur_node->get_child((uint8_t)i);
                cur_key.push_back((uint8_t)i);
                child_key = children_count - 1;
                i = child_key;
            }
        }
        return;
    }
}
