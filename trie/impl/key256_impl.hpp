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

#include "../key.hpp"

template<> void trie::key<256>::init(const void* data, std::size_t len)
{
    _key.resize(len);
    this->_size = len;
    for (std::size_t i = 0; i < len; i++)
    {
        _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}

template<> void trie::key<256>::init(const std::string& string_key)
{
    _key.resize(string_key.length());
    this->_size = string_key.length();
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        _key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }
}

template<> std::string trie::key<256>::to_string() const
{
    std::string result;
    for (std::size_t i = 0; i < _key.size(); i++)
    {
        result.push_back(_key.at(i));
    }
    return result;
}

template<> std::string trie::key<256>::to_hex_string() const
{
    std::string result("0x");
    for (std::size_t i = 0; i < this->size(); i++)
    {
        result.push_back(__4b_int_to_hex_char[this->get_element(i) & 0xF]);
        result.push_back(__4b_int_to_hex_char[this->get_element(i) >> 4 & 0xF]);
    }
    return result;
}

template<> uint8_t trie::key<256>::get_element(std::size_t index) const
{
    return this->_key.at(index);
}

template<> std::size_t trie::key<256>::size() const
{
    return this->_key.size();
}

template<> void trie::key<256>::push_back(uint8_t data)
{
    this->_key.push_back(data);
    this->_size++;
}

template<> void trie::key<256>::pop_back()
{
    this->_key.pop_back();
    this->_size--;
}

template<> void trie::key<256>::clear()
{
    this->_key.clear();
    this->_size = 0;
}

/*template<> std::size_t trie::key<256>::export_size() const
{
    return this->_key.size();
}

template<> void trie::key<256>::export_key(void* buffer, std::size_t buflen) const
{
    for (std::size_t i = 0; i < std::min(this->export_size(), buflen); i++)
    {
        ((uint8_t*)buffer)[i] = this->_key.at(i);
    }
}*/
