/**
* @file     trie/256/key.hpp
* @brief    include file for the 256-children key class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../basic_key.hpp"

namespace trie
{
    namespace trie256
    {
        class key : public ::trie::basic_key
        {
        private:
            std::vector<element_t> _key;
        public:
            constexpr static std::size_t children_count = 256;

            key();
            key(void* data, std::size_t len);
            key(const std::string& string_key);

            void init(void* data, std::size_t len);
            void init(const std::string& string_key);

            std::string to_string() const;
            std::string to_hex_string() const;
            element_t get_element(std::size_t index) const;
            std::size_t size() const;
            void push_back(element_t data);
            void pop_back();
            void clear();
        };
    } // namespane trie16
} // namespace trie
