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

// include definition headers
#include "basic_key.hpp"
#include "256/key.hpp"
#include "16/key.hpp"
#include "basic_trie.hpp"

// include implementation headers
#include "impl/256/key_impl.hpp"
#include "impl/16/key_impl.hpp"
#include "impl/basic_trie_impl.hpp"
#include "impl/basic_node_iterator_impl.hpp"

namespace trie
{
    using key256_t = trie::trie256::key;
    using key16_t = trie::trie16::key;

    template<typename value_t> using trie256_t = basic_trie<trie::key256_t, value_t>;
    template<typename value_t> using trie16_t = basic_trie<trie::key16_t, value_t>;
} // namespace trie
