/**
* @file     trie/impl/key16_impl.hpp
* @brief    include file for the 16 children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_key.hpp"


template<> void trie::basic_key<16>::init(const void* data, std::size_t len)
{
    _key.resize(len);
    this->_size = len * 2;
    for (std::size_t i = 0; i < len; i++)
    {
        _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}

template<> void trie::basic_key<16>::init(const std::string& string_key)
{
    _key.resize(string_key.length());
    this->_size = string_key.length() * 2;
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        _key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }
}

template<> std::string trie::basic_key<16>::to_string() const
{
    std::string result;
    for (std::size_t i = 0; i < _key.size(); i++)
    {
        result.push_back(_key.at(i));
    }
    return result;
}

template<> std::string trie::basic_key<16>::to_hex_string() const
{
    std::string result("0x");
    for (std::size_t i = 0; i < this->_key.size(); i++)
    {
        result.push_back(__4b_int_to_hex_char[this->_key.at(i) & 0xF]);
        result.push_back(__4b_int_to_hex_char[this->_key.at(i) >> 4 & 0xF]);
    }
    return result;
}

template<> uint8_t trie::basic_key<16>::get_element(std::size_t index) const
{
    // get raw binary data from key bytes
    uint8_t data = this->_key.at(index >> 1);
    if (index % 2 == 0)
    {
        // even, use lower bits
        data = data & 0xF;
    }
    else
    {
        // odd, use higher bits
        data = (data & 0xF0) >> 4;
    }
    return data; // the value can only hold values in the range 0x0 to 0xF because of the code above
}

template<> std::size_t trie::basic_key<16>::size() const
{
    return this->_size;
}

template<> void trie::basic_key<16>::push_back(uint8_t data)
{
    if (this->_size % 2 == 0)
    {
        // even, new byte needed
        // eg current key is something like 0x12'34 and a 5 should be appended, the key needs an extra byte to store 0x12'34'5?
        this->_key.push_back(data & 0xF);
        this->_size++;
    }
    else
    {
        // odd, data needs to be stffed into the last byte
        // eg. the current key is something like 0x12'34'5? and a 6 should be appended, the element can be inserted into the unused bits of the last byte
        this->_key.at(this->_key.size() - 1) |= ((data << 4) & 0xF0);
        this->_size++;
    }
}

template<> void trie::basic_key<16>::pop_back()
{
    if (this->_size % 2 == 0)
    {
        // even, removal does not remove a byte, but the last 4 bits are cleared
        // eg. current key is something like 0x12'34'56 , the 6 should be cleared
        this->_key.at(this->_key.size() - 1) &= 0xF;
        this->_size--;
    }
    else
    {
        // odd, removal will remove the last byte, clearing is not required since the byte is discarded
        // eg. key is something like 0x12'34'5? , the ? is unused, so the entire byte should be removed
        this->_key.pop_back();
        this->_size--;
    }
}

template<> void trie::basic_key<16>::clear()
{
    this->_key.clear();
    this->_size = 0;
}

/*template<> std::size_t trie::basic_key<16>::export_size() const
{
    return this->_key.size();
}

template<> void trie::basic_key<16>::export_key(void* buffer, std::size_t buflen) const
{
    for (std::size_t i = 0; i < std::min(this->export_size(), buflen); i++)
    {
        ((uint8_t*)buffer)[i] = this->_key.at(i);
    }
}*/
