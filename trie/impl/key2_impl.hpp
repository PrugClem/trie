/**
* @file     trie/impl/key2_impl.hpp
* @brief    include file for the 2 children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_key.hpp"

template<> void trie::basic_key<2>::init(const void* data, std::size_t len)
{
    _key.resize(len);
    this->_size = len * 8;
    for (std::size_t i = 0; i < len; i++)
    {
        _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}

template<> void trie::basic_key<2>::init(const std::string& string_key)
{
    _key.resize(string_key.length());
    this->_size = string_key.length() * 8;
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        _key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }
}

template<> std::string trie::basic_key<2>::to_string() const
{
    std::string result;
    for (std::size_t i = 0; i < _key.size(); i++)
    {
        result.push_back(_key.at(i));
    }
    return result;
}

template<> std::string trie::basic_key<2>::to_hex_string() const
{
    std::string result("0x");
    for (std::size_t i = 0; i < this->_key.size(); i++)
    {
        result.push_back(__4b_int_to_hex_char[this->_key.at(i) & 0xF]);
        result.push_back(__4b_int_to_hex_char[(this->_key.at(i) >> 4) & 0xF]);
    }
    return result;
}

template<>  uint8_t  trie::basic_key<2>::get_element(std::size_t index) const
{
    uint8_t data = this->_key.at(index >> 3);
    std::size_t shift_amount = (7 - (index % 8));
    uint8_t mask = 0b1 << shift_amount;
    return (data & mask) >> shift_amount;
}

template<> std::size_t trie::basic_key<2>::size() const
{
    return this->_size;
}

template<> void trie::basic_key<2>::push_back(uint8_t data)
{
    std::size_t shift_amount = (7 - (_size % 8));
    uint8_t mask = 0b1 << shift_amount;

    if ((_size % 8) == 0)
    {
        this->_key.push_back((data << shift_amount) & mask);
    }
    else
    {
        this->_key.at(this->_key.size() - 1) |= ((data << shift_amount) & mask);
    }
    ++_size;
}

template<> void trie::basic_key<2>::pop_back()
{
    std::size_t shift_amount = (7 - (--_size % 8));
    uint8_t mask = ~(0b1 << shift_amount);

    if ((_size % 8) == 0)
    {
        this->_key.pop_back();
    }
    else
    {
        this->_key.at(this->_key.size() - 1) &= mask;
    }
}

template<> void trie::basic_key<2>::clear()
{
    this->_key.clear();
    this->_size = 0;
}
