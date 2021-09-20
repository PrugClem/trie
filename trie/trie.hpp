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
#include "basic_key.hpp"
#include "basic_trie.hpp"

// implementation include files
#include "impl/key256_impl.hpp"
#include "impl/key16_impl.hpp"
#include "impl/key4_impl.hpp"
#include "impl/key2_impl.hpp"
#include "impl/basic_trie_impl.hpp"
#include "impl/basic_node_iterator_impl.hpp"
#include "impl/basic_value_iterator_impl.hpp"

namespace trie
{
    // trie usings
    template<typename value_t> using trie256 = trie::basic_trie<256, value_t>;
    template<typename value_t> using trie16 = trie::basic_trie<16, value_t>;
    template<typename value_t> using trie4 = trie::basic_trie<4, value_t>;
    template<typename value_t> using trie2 = trie::basic_trie<2, value_t>;

    // key usings
    using key256 = trie::basic_key<256>;
    using key16 = trie::basic_key<16>;
    using key4 = trie::basic_key<4>;
    using key2 = trie::basic_key<2>;
}
