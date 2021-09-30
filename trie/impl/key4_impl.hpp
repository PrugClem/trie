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

template<> uint8_t trie::basic_key<4>::get_element(std::size_t index) const
{
    // this inactive preprocessor block is here to help understanding the code below
    // if you can understand this method, you should be able to understand the method
    //  for the 2-children key class, since I used the same priciple
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
    uint8_t data = this->_key.at(index >> 2); // right shift 2 is the same as divide by 4, but faster (some compilers will optimise to the same code anyways)
    std::size_t shift_amount = (3 - (index % 4)) * 2; // calculate the amount of bits to shift, see the inactive code above to understand this
    uint8_t mask = 0b11 << shift_amount; // calculate the bit mask to extrct the correct bytes from the extracted byte
    return (data & mask) >> shift_amount; // extract the bits from the byte (multiple variables are used for code readability)
#endif
}

template<> std::size_t trie::basic_key<4>::size() const
{
    return this->_size;
}

template<> void trie::basic_key<4>::push_back(uint8_t data)
{
    // this inactive preprocessor block is here to help understanding the code below
    // if you can understand this method, you should be able to understand the method
    //  for the 2-children key class, since I used the same priciple
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
    std::size_t shift_amount = (3 - (_size % 4)) * 2; // calculate the amount of bits to shift the mask, see the code above to understand this
    uint8_t mask = 0b11 << shift_amount; // calculate the bit mask to not corrupt the other data in the key

    if((_size % 4) == 0) // when adding a new key element, it is sometimes reqired to add a new byte to the key container
    {
        this->_key.push_back((data << shift_amount) & mask); // when pushing a new byte, all unused bits are set to zero
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
    // this inactive preprocessor block is here to help understanding the code below
    // if you can understand this method, you should be able to understand the method
    //  for the 2-children key class, since I used the same priciple
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
    std::size_t shift_amount = (3 - (--_size % 4)) * 2; // calculate the amount of bits to shift the mask, see the code above to understand this
    uint8_t mask = ~(0b11 << shift_amount); // calculate the mask to clear out the unused bits

    if ((_size % 4) == 0) // if the byte is emptied out, it can be removed from the container
    {
        this->_key.pop_back();
    }
    else
    {
        // however, if the byte persists in the container, it is required to mask the now unused bytes to 0
        // new key elements are binary or'd into the key, which will cause problems if the unused bits are not zero'd out
        this->_key.at(this->_key.size() - 1) &= mask;
    }
#endif
}

template<> void trie::basic_key<4>::clear()
{
    this->_key.clear();
    this->_size = 0;
}
