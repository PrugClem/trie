/**
* @file     trie/key.hpp
* @brief    include file for key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <array>
#include <string>
#include <memory>
#include <vector>
#include <cstddef>

namespace trie
{
    template<std::size_t children_count>
    class basic_key
    {
    protected:
        static constexpr const char* __4b_int_to_hex_char = "0123456789ABCDEF";
        std::vector<uint8_t> _key;
        std::size_t _size;
    public:
        basic_key() { this->_key.clear(); this->_size = 0; }
        basic_key(const void* data, std::size_t len) { this->init(data, len); }
        basic_key(const std::string& string_key) { this->init(string_key); }
        basic_key(const basic_key& other) { this->_key = other._key; this->_size = other._size; }
        ~basic_key() {}

        void init(const void* data, std::size_t len);
        void init(const std::string& string_key);

        std::string to_string() const;
        [[deprecated("This method appears to have some bugs")]] std::string to_hex_string() const;
        uint8_t get_element(std::size_t index) const;
        std::size_t size() const;
        void push_back(uint8_t data);
        void pop_back();
        void clear();

        //std::size_t export_size() const;
        //void export_key(void* buffer, std::size_t buflen) const;
        
    }; // class key
}; // namespace trie
