/**
* @file     trie/basic_key.hpp
* @brief    include file for key interface class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <cstddef>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <type_traits>

namespace trie
{
    using element_t = uint8_t;
    class basic_key
    {
    public:
        virtual void init(void* data, std::size_t len) = 0;
        virtual void init(const std::string& string_key) = 0;

        virtual std::string to_string() const = 0;
        virtual std::string to_hex_string() const = 0;
        virtual element_t get_element(std::size_t index) const = 0;
        virtual std::size_t size() const = 0;
        virtual void push_back(element_t data) = 0;
        virtual void pop_back() = 0;
        virtual void clear() = 0;
    };
} // namespace trie
