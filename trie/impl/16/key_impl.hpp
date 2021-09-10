/**
* @file     trie/impl/16/key_impl.hpp
* @brief    include file for the 16-children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../16/key.hpp"

trie::trie16::key::key()
{
    this->_key.clear();
    this->_size = 0;
}
trie::trie16::key::key(void* data, std::size_t len)
{
    this->init(data, len);
}
trie::trie16::key::key(const std::string& string_key)
{
    this->init(string_key);
}

void trie::trie16::key::init(void* data, std::size_t len)
{
    this->_key.resize(len);
    this->_size = len * 2;
    for (std::size_t i = 0; i < len; i++)
    {
        this->_key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}
void trie::trie16::key::init(const std::string& string_key)
{
    this->_key.resize(string_key.length());
    this->_size = string_key.length() * 2;
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        this->_key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }

}

std::string trie::trie16::key::to_string() const
{
    std::string result;
    for (std::size_t i = 0; i < _key.size(); i++)
    {
        result.push_back(_key.at(i));
    }
    return result;
}
std::string trie::trie16::key::to_hex_string() const
{
    static constexpr const char* __4b_int_to_hex_char = "0123456789ABCDEF";
    std::string result("0x");
    for (std::size_t i = 0; i < this->size(); i++)
    {
        result.push_back(__4b_int_to_hex_char[this->get_element(i) & 0xF]);
    }
    return result;
}
trie::element_t trie::trie16::key::get_element(std::size_t index) const
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
std::size_t trie::trie16::key::size() const
{
    return this->_size;
}
void trie::trie16::key::push_back(element_t data)
{
    if (this->_size % 2 == 0)
    {
        // even, new byte needed
        this->_key.push_back(data & 0xF);
        this->_size++;
    }
    else
    {
        // odd, data needs to be stffed into the last byte
        this->_key.at(this->_key.size() - 1) |= ((data << 4) & 0xF0);
        this->_size++;
    }
}
void trie::trie16::key::pop_back()
{
    if (this->_size % 2 == 0)
    {
        // even, removal does not remove a byte, but the last 4 bits are cleared
        this->_key.at(this->_key.size() - 1) &= 0xF;
        this->_size--;
    }
    else
    {
        // odd, removal will remove the last byte, clearing is not required since the byte is discarded
        this->_key.pop_back();
        this->_size--;
    }
}
void trie::trie16::key::clear()
{
    this->_key.clear();
    this->_size = 0;
}
