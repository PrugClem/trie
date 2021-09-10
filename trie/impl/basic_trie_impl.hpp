/**
* @file     trie/impl/basic_trie.hpp
* @brief    include file for the implementations for the main trie class
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
std::shared_ptr<typename trie::basic_trie<key_t, value_t>::node>&
trie::basic_trie<key_t, value_t>::node::get_child(trie::element_t element)
{
    return this->children.at(element);
}

template<typename key_t, typename value_t>
std::shared_ptr<typename trie::basic_trie<key_t, value_t>::node>
trie::basic_trie<key_t, value_t>::get_node(const key_t& key)
{
    std::shared_ptr<node> helper = _root, child;
    for (std::size_t i = 0; i < key.size(); i++)
    {
        child = helper->get_child(key.get_element(i)); // get the child item
        if (child == nullptr) return nullptr;
        helper = child; // set the helper pointer to the child and proceed to the next key element
    }
    return helper;
}

template<typename key_t, typename value_t>
std::shared_ptr<typename trie::basic_trie<key_t, value_t>::node>
trie::basic_trie<key_t, value_t>::add_node(const key_t& key)
{
    std::shared_ptr<node> helper = _root, child;
    // search for the requested child node and create any nodes that are missing
    for (std::size_t i = 0; i < key.size(); i++)
    {
        child = helper->get_child(key.get_element(i)); // get the child item
        if (child == nullptr)
        {
            child = helper->get_child(key.get_element(i)) = std::make_shared<node>(); // if the child item is empty, allocate a new one and set the helper pointer to it
        }
        helper = child;
    }
    return helper;
}

template<typename key_t, typename value_t>
bool
trie::basic_trie<key_t, value_t>::has_node(const key_t& key)
{
    return this->get_node(key) != nullptr;
}

template<typename key_t, typename value_t>
std::shared_ptr<value_t>&
trie::basic_trie<key_t, value_t>::at(const key_t& key)
{
    std::shared_ptr<node> _node = this->get_node(key); // get the node containing the child
    if(_node == nullptr) throw std::out_of_range("Trie does not have the requested child"); // throw exception if child does not exists
    return _node->data; // return data
}

template<typename key_t, typename value_t>
std::shared_ptr<value_t>&
trie::basic_trie<key_t, value_t>::operator[](const key_t& key)
{
    std::shared_ptr<node> _node = this->add_node(key); // get / add the node containing the child
    return _node->data; // return data
}

template<typename key_t, typename value_t>
bool
trie::basic_trie<key_t, value_t>::insert(const key_t& key, std::shared_ptr<value_t> value)
{
    std::shared_ptr<node> helper = this->add_node(key);
    if (helper->data == nullptr)
    {
        // if the node does not contain data, set the data and return true
        helper->data = value;
        return true;
    }
    else
    {
        // if the child already contained data, return false
        return false;
    }
}

template<typename key_t, typename value_t>
trie::basic_trie<key_t, value_t>
trie::basic_trie<key_t, value_t>::subtrie(const key_t& key)
{
    std::shared_ptr<node> newroot = this->get_node(key);
    if(newroot == nullptr) throw std::out_of_range("Trie does not have the requested child");
    return ::trie::basic_trie<key_t, value_t>(newroot);
}

template<typename key_t, typename value_t>
trie::basic_trie<key_t, value_t>
trie::basic_trie<key_t, value_t>::clone()
{
    basic_trie<key_t, value_t> _clone;
    for (auto iter = this->node_begin(); iter != this->node_end(); iter++)
        _clone.insert(iter.get_key(), iter.get_data());
    return _clone;
}
