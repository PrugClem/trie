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
    /**
     * @brief holds a trie's key. This class is introduced in order to reduce the amount of code required to make the trie work.
     *  This class is responsible for slicing up a chunk of bytes into 1/2/4/8-bit sized chunks (key element)
     *  The template argument holds the trie's children count, which also represents the number of possible states for one key element.
     *  The  implementations make space-efficient use of the key by putting as many key elements into one byte as possible
     * 
     * @tparam children_count number of possible states for one key element. MUST be one of: 2, 4, 16, 256
     */
    template<std::size_t children_count>
    class basic_key
    {
    protected:
        /**
        * @brief conversion array to convert stuff to hexadecimal
        */
        static constexpr const char* __4b_int_to_hex_char = "0123456789ABCDEF";
        std::vector<uint8_t> _key;
        std::size_t _size;
    public:
        /**
        * @brief default constructor
        */
        basic_key() { this->_key.clear(); this->_size = 0; }
        /**
        * @brief construct a key and load bytes into it
        */
        basic_key(const void* data, std::size_t len) { this->init(data, len); }
        /**
        * @brief construct a key and load anull-terminated string into it
        */
        basic_key(const std::string& string_key) { this->init(string_key); }
        /**
        * @brief copy ctor
        */
        basic_key(const basic_key& other) { this->_key = other._key; this->_size = other._size; }
        ~basic_key() {}

        /**
        * @brief reinitializes the key and loads new bytes into it
        * @param data pointer to the sat of the binary data
        * @param len length of the binary data in bytes
        * @throws std::bad_alloc from std::vector::resize
        */
        void init(const void* data, std::size_t len);
        /**
        * @brief reinitializes the key and loads a null-terminated string into it, the terminatingn char '\0' is not loaded into the key
        * @param string_key the string to load into the key
        * @throws std::bad_alloc from std::vector::resize
        */
        void init(const std::string& string_key);

        /**
        * @brief get a key element (slice of the key)
        * @param index index of the element, must be smaller than the .size() of the key
        * @return the key element at index 'index'
        * @throw std::out_of_range from std::vector::at
        */
        uint8_t get_element(std::size_t index) const;
        /**
         * @brief get the key's number of key elements (might be different than the number of bytes loaded into the key)
         * If the number of bytes in the key is needed, use the method 'export_size' instead
         */
        std::size_t size() const;
        /**
         * @brief append another key element to the key
         * @param data the key element to append
         * @throws std::bad_alloc from std::vector::push_back
         */
        void push_back(uint8_t data);
        /**
         * @brief remove the last key element from the key
         */
        void pop_back();
        /**
         * @brief resets the entire key into an empty state.
         */
        void clear();

        /**
         * @brief converts the key to a string, the string miight contain non-printable characters or be incomplete (if a '\0' is present in the key)
         * 
         * @return string representation of the key
         */
        std::string to_string() const;
        [[deprecated("This method appears to have some bugs")]] std::string to_hex_string() const;
        /**
         * @brief get the number of bytes needed to represent or export the key
         * @return the minimum number of bytes required to export the key
         */
        std::size_t export_size() const;
        /**
         * @brief exports the key into a buffer. If the buffer is too small, the export process will not be able to export all bytes from the key and the exported key is incomplete
         * 
         * @param buffer where to export the key to
         * @param buflen hohow big the allocated buffer isiin bytes
         */
        void export_key(void* buffer, std::size_t buflen) const;
        
    }; // class key

    template<std::size_t children_count>
    std::string basic_key<children_count>::to_string() const
    {
        std::string result;
        for (std::size_t i = 0; i < _key.size(); i++)
        {
            result.push_back(_key.at(i));
        }
        return result;
    }

    template<std::size_t children_count>
    std::string trie::basic_key<children_count>::to_hex_string() const
    {
        std::string result("0x");
        for (std::size_t i = 0; i < this->_key.size(); i++)
        {
            result.push_back(__4b_int_to_hex_char[(this->_key.at(i) >> 0) & 0xF]);
            result.push_back(__4b_int_to_hex_char[(this->_key.at(i) >> 4) & 0xF]);
        }
        return result;
    }

    template <std::size_t children_count>
    std::size_t basic_key<children_count>::export_size() const
    {
        return this->_key.size();
    }

    template <std::size_t children_count>
    void basic_key<children_count>::export_key(void *buffer, std::size_t buflen) const
    {
        memcpy(buffer, this->_key.data(), std::min(buflen, this->_key.size());
    }
}; // namespace trie
