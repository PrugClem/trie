/**
* @file     trie/impl/basic_trie_impl.hpp
* @brief    include file for main implementations for the trie class
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
std::shared_ptr< typename trie::basic_trie<children_count, value_t>::node >&
trie::basic_trie<children_count, value_t>::node::get_child(std::size_t key_element)
{
    return this->children.at(key_element);
}

template<std::size_t children_count, typename value_t>
std::shared_ptr< typename trie::basic_trie<children_count, value_t>::node >
trie::basic_trie<children_count, value_t>::get_node(const key_t& key)
{
    std::shared_ptr<node> helper = _root, child;
    for (std::size_t i = 0; i < key.size(); i++)
    {
        child = helper->get_child(key.get_element(i)); // get the child item
        if (child == nullptr) return nullptr; // return a nullptr if the node does not exist
        helper = child; // set the helper pointer to the child and proceed to the next key element
    }
    return helper;
}

template<std::size_t children_count, typename value_t>
std::shared_ptr< typename trie::basic_trie<children_count, value_t>::node >
trie::basic_trie<children_count, value_t>::add_node(const key_t& key)
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

template<std::size_t children_count, typename value_t>
std::shared_ptr< typename trie::basic_trie<children_count, value_t>::node >&&
trie::basic_trie<children_count, value_t>::extract_node(const key_t& key)
{
    std::shared_ptr<node> helper = _root, child;
    for(std::size_t i = 0; i < key.size()-1; i++) // do not go all the way to the desired node, because this method needs to modify the parent node to extract it
    {
        child = helper->get_child(key.get_element(i)); // get the child
        if (child == nullptr) return nullptr; // return a nullptr if the node does not extist
        helper = child; // go to the child pointer
    } // at this point, the helper pointer points to the parent node of the requested node, the only thing left to do is to acutally extract the node
    return std::move(helper->get_child(key.get_element(key.size() - 1))); // return the pointer to the desired node and remove the reference from the trie itself (handld by std::move(std::shared_ptr) )
}

template<std::size_t children_count, typename value_t>
bool
trie::basic_trie<children_count, value_t>::has_node(const key_t& key)
{
    return this->get_node(key) != nullptr;
}

template<std::size_t children_count, typename value_t>
std::shared_ptr<value_t>&
trie::basic_trie<children_count, value_t>::at(const key_t& key)
{
    std::shared_ptr<node> _node = this->get_node(key); // get the node containing the child
    if (_node == nullptr) throw std::out_of_range("Trie does not have the requested child"); // throw exception if child does not exists
    return _node->data; // return data
}

template<std::size_t children_count, typename value_t>
std::shared_ptr<value_t>&
trie::basic_trie<children_count, value_t>::operator[](const key_t& key)
{
    std::shared_ptr<node> _node = this->add_node(key); // get / add the node containing the child
    return _node->data; // return data
}

template<std::size_t children_count, typename value_t>
bool
trie::basic_trie<children_count, value_t>::insert(const key_t& key, std::shared_ptr<value_t> value)
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

template<std::size_t children_count, typename value_t>
bool
trie::basic_trie<children_count, value_t>::erase(const key_t& key)
{
    // extract the node from the trie, when leaving the method scope, the pointer is deconstructed.
    //  Since the pointer was std::move'd out of the trie, the node is removed from the trie
    std::shared_ptr<node> helper = this->extract_node(key); 
    return helper != nullptr; // return true to indicate that the node was removed, return false to indicate that the node did not exist
}

template<std::size_t children_count, typename value_t>
void
trie::basic_trie<children_count, value_t>::merge(trie::basic_trie<children_count, value_t>& source)
{
    // This method has to collect all nodes in reverse order from the source trie and then add them in forward order to this trie.
    std::shared_ptr<node> helper;
    std::vector<std::pair<key_t, std::shared_ptr<node> > > node_stack;
    for (auto iter = source.node_rbegin(); iter != source.node_rend(); iter++)
    {
        helper = this->get_node(iter.get_key()); // check if this trie has the node
        if (helper == nullptr) // this trie does not have the node, extract if and push it onto the node stack
        {
            node_stack.push_back(std::make_pair(iter.get_key(), source.extract_node(iter.get_key())));
        }
    }
    while (!node_stack.empty())
    {
        std::pair<key_t, std::shared_ptr<node> >& ref = node_stack.back(); // get a reference to the stored values
        helper = this->add_node(ref.first); // add the jnode to this trie
        helper->children = ref.second->children; // set the children vector
        helper->data = ref.second->data; // set the data value
        node_stack.pop_back(); // emove the node from the stack to mark it as done
    }
}

template<std::size_t children_count, typename value_t>
trie::basic_trie<children_count, value_t>
trie::basic_trie<children_count, value_t>::subtrie(const key_t& key)
{
    std::shared_ptr<node> newroot = this->get_node(key);
    if (newroot == nullptr) throw std::out_of_range("Trie does not have the requested child");
    return ::trie::basic_trie<children_count, value_t>(newroot);
}

template<std::size_t children_count, typename value_t>
trie::basic_trie<children_count, value_t>
trie::basic_trie<children_count, value_t>::clone()
{
    basic_trie<children_count, value_t> _clone;
    for (auto iter = this->node_begin(); iter != this->node_end(); iter++)
        _clone.insert(iter.get_key(), iter.get_data());
    return _clone;
}

template<std::size_t children_count, typename value_t>
void
trie::basic_trie<children_count, value_t>::clear()
{
    this->_root = std::make_shared<node>();
}
