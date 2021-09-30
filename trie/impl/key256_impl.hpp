/**
* @file     trie/impl/key256_impl.hpp
* @brief    include file for the 256 children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_key.hpp"

template<> void trie::basic_key<256>::init(const void* data, std::size_t len)
{
    _key.resize(len);
    this->_size = len;
    for (std::size_t i = 0; i < len; i++)
    {
        _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}

template<> void trie::basic_key<256>::init(const std::string& string_key)
{
    _key.resize(string_key.length());
    this->_size = string_key.length();
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        _key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }
}

template<> uint8_t trie::basic_key<256>::get_element(std::size_t index) const
{
    return this->_key.at(index);
}

template<> std::size_t trie::basic_key<256>::size() const
{
    return this->_key.size();
}

template<> void trie::basic_key<256>::push_back(uint8_t data)
{
    this->_key.push_back(data);
    this->_size++;
}

template<> void trie::basic_key<256>::pop_back()
{
    this->_key.pop_back();
    this->_size--;
}

template<> void trie::basic_key<256>::clear()
{
    this->_key.clear();
    this->_size = 0;
}
