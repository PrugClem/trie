/**
* @file     trie/impl/key4_impl.hpp
* @brief    include file for the 4 children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_key.hpp"

template<> void trie::basic_key<4>::init(const void* data, std::size_t len)
{
    _key.resize(len);
    this->_size = len * 4;
    for (std::size_t i = 0; i < len; i++)
    {
        _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}

template<> void trie::basic_key<4>::init(const std::string& string_key)
{
    _key.resize(string_key.length());
    this->_size = string_key.length() * 4;
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        _key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }
}

template<> std::string trie::basic_key<4>::to_string() const
{
    std::string result;
    for (std::size_t i = 0; i < _key.size(); i++)
    {
        result.push_back(_key.at(i));
    }
    return result;
}

template<> std::string trie::basic_key<4>::to_hex_string() const
{
    std::string result("0x");
    for (std::size_t i = 0; i < this->_key.size(); i++)
    {
        result.push_back(__4b_int_to_hex_char[this->_key.at(i) & 0xF]);
        result.push_back(__4b_int_to_hex_char[(this->_key.at(i) >> 4) & 0xF]);
    }
    return result;
}

template<> uint8_t trie::basic_key<4>::get_element(std::size_t index) const
{
#if 0
    uint8_t data = this->_key.at(index >> 2);
    switch (index % 4)
    {
    case 0:
        data = (data & 0b1100'0000) >> 6;
        break;
    case 1:
        data = (data & 0b0011'0000) >> 4;
        break;
    case 2:
        data = (data & 0b0000'1100) >> 2;
        break;
    case 3:
        data = (data & 0b0000'0011) >> 0;
        break;
    }
    return data;
#else
    uint8_t data = this->_key.at(index >> 2);
    std::size_t shift_amount = (3 - (index % 4)) * 2;
    uint8_t mask = 0b11 << shift_amount;
    return (data & mask) >> shift_amount;
#endif
}

template<> std::size_t trie::basic_key<4>::size() const
{
    return this->_size;
}

template<> void trie::basic_key<4>::push_back(uint8_t data)
{
#if 0
    switch (_size++ % 4)
    {
    case 0:
        this->_key.push_back((data << 6) & 0b1100'0000);
        break;
    case 1:
        this->_key.at(this->_key.size() - 1) |= ((data << 4) & 0b0011'0000);
        break;
    case 2:
        this->_key.at(this->_key.size() - 1) |= ((data << 2) & 0b0000'1100);
        break;
    case 3:
        this->_key.at(this->_key.size() - 1) |= ((data << 0) & 0b0000'0011);
        break;
    }
#else
    std::size_t shift_amount = (3 - (_size % 4)) * 2;
    uint8_t mask = 0b11 << shift_amount;

    if((_size % 4) == 0)
    {
        this->_key.push_back((data << shift_amount) & mask);
    }
    else
    {
        this->_key.at(this->_key.size() - 1) |= ((data << shift_amount) & mask);
    }
    ++_size;
#endif
}

template<> void trie::basic_key<4>::pop_back()
{
#if 0
    switch (--_size % 4)
    {
    case 0:
        this->_key.pop_back();
        break;
    case 1:
        this->_key.at(this->_key.size() - 1) &= 0b1100'0000;
        break;
    case 2:
        this->_key.at(this->_key.size() - 1) &= 0b1111'0000;
        break;
    case 3:
        this->_key.at(this->_key.size() - 1) &= 0b1111'1100;
        break;
    }
#else
    std::size_t shift_amount = (3 - (--_size % 4)) * 2;
    uint8_t mask = ~(0b11 << shift_amount);

    if ((_size % 4) == 0)
    {
        this->_key.pop_back();
    }
    else
    {
        this->_key.at(this->_key.size() - 1) &= mask;
    }
#endif
}

template<> void trie::basic_key<4>::clear()
{
    this->_key.clear();
    this->_size = 0;
}
