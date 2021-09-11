/**
* @file     trie/trie.hpp
* @brief    main include file for the header-only trie implementation
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

// definitions include files
#include "key.hpp"
#include "basic_trie.hpp"

// implementation include files
#include "impl/key256_impl.hpp"
#include "impl/key16_impl.hpp"
#include "impl/basic_trie_impl.hpp"
#include "impl/basic_node_iterator_impl.hpp"
#include "impl/basic_value_iterator_impl.hpp"

namespace trie
{
    template<typename value_t> using trie256 = trie::basic_trie<256, value_t>;
    template<typename value_t> using trie16 = trie::basic_trie<16, value_t>;
}
