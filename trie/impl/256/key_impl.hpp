/**
* @file     trie/impl/256/key_impl.hpp
* @brief    include file for the 16-children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../256/key.hpp"

trie::trie256::key::key()
{
    this->_key.clear();
}
trie::trie256::key::key(void* data, std::size_t len)
{
    this->init(data, len);
}
trie::trie256::key::key(const std::string& string_key)
{
    this->init(string_key);
}


void trie::trie256::key::init(void* data, std::size_t len)
{
    _key.resize(len);
    for (std::size_t i = 0; i < len; i++)
    {
        _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
    }
}
void trie::trie256::key::init(const std::string& string_key)
{
    _key.resize(string_key.length());
    for (std::size_t i = 0; i < string_key.size(); i++)
    {
        _key.at(i) = string_key.at(i); // copy every ASCII character into the key
    }

}

std::string trie::trie256::key::to_string() const
{
    std::string result;
    for (std::size_t i = 0; i < _key.size(); i++)
    {
        result.push_back(_key.at(i));
    }
    return result;
}
std::string trie::trie256::key::to_hex_string() const
{
    static constexpr const char* __4b_int_to_hex_char = "0123456789ABCDEF";
    std::string result("0x");
    for (std::size_t i = 0; i < this->size(); i++)
    {
        result.push_back(__4b_int_to_hex_char[this->_key.at(i) & 0xF]);
        result.push_back(__4b_int_to_hex_char[this->_key.at(i) >> 4 & 0xF]);
    }
    return result;
}
trie::element_t trie::trie256::key::get_element(std::size_t index) const
{
    return this->_key.at(index);
}
std::size_t trie::trie256::key::size() const
{
    return this->_key.size();
}
void trie::trie256::key::push_back(element_t data)
{
    this->_key.push_back(data);
}
void trie::trie256::key::pop_back()
{
    this->_key.pop_back();
}
void trie::trie256::key::clear()
{
    this->_key.clear();
}
