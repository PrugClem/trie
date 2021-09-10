/**
* @file     trie/basic_trie.hpp
* @brief    include file for the definitions for the main trie class
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "basic_key.hpp"

namespace trie
{
    template<typename key_t, typename value_t>
    class basic_trie
    {
        static_assert(std::is_base_of<trie::basic_key, key_t>::value, "Invalid key type provided, key type must be inherited from trie::basic_key");
    protected:
        struct node
        {
            std::array<std::shared_ptr<node>, key_t::children_count> children;
            std::shared_ptr<value_t> data;

            std::shared_ptr<node>& get_child(std::uint8_t key_element);
        };

        std::shared_ptr<node> _root;

        std::shared_ptr<node> get_node(const key_t& _key);
        std::shared_ptr<node> add_node(const key_t& _key);

        basic_trie(std::shared_ptr<node> root) { this->_root = root; }

        struct basic_node_iterator
        {
        protected:
            std::shared_ptr<node> root_node{ nullptr };
            mutable std::shared_ptr<node> cur_node{ nullptr };
            mutable key_t cur_key;
            mutable trie::element_t child_element{ 0 };

        public:
            basic_node_iterator() {}
            basic_node_iterator(std::shared_ptr<node> root_node) : root_node(root_node) {}
            virtual ~basic_node_iterator() {}

            bool operator==(const basic_node_iterator& other) const;
            bool operator!=(const basic_node_iterator& other) const;

            operator bool() const { return this->root_node != nullptr; }
            bool operator!() const { return this->root_node == nullptr; }

            const key_t get_key() const { return this->cur_key; }
            std::shared_ptr<value_t> get_data() { return this->cur_node->data; }
            const std::shared_ptr<value_t> get_data() const { return this->cur_node->data; }

        protected:
            void next_node() const;
            void prev_node() const;
        }; // struct basic_node_iterator

        class node_iterator : public basic_node_iterator
        {
        public:
            node_iterator() {}
            node_iterator(std::shared_ptr<node> root_node) : basic_node_iterator(root_node) {}
            ~node_iterator() {}

            const node_iterator& operator++() const { this->next_node(); return *this; }
            const node_iterator operator++(int) const { node_iterator copy = *this; this->next_node(); return copy; }
            const node_iterator& operator--() const { this->prev_node(); return *this; }
            const node_iterator operator--(int) const { node_iterator copy = *this; this->prev_node(); return copy; }
        }; // class node_iterator

        class reverse_node_iterator : public basic_node_iterator
        {
        public:
            reverse_node_iterator() {}
            reverse_node_iterator(std::shared_ptr<node> root_node) : basic_node_iterator(root_node) {}
            ~reverse_node_iterator() {}

            const reverse_node_iterator& operator++() const { this->next_node(); return *this; }
            const reverse_node_iterator operator++(int) const { reverse_node_iterator copy = *this; this->prev_node(); return copy; }
            const reverse_node_iterator& operator--() const { this->prev_node(); return *this; }
            const reverse_node_iterator operator--(int) const { reverse_node_iterator copy = *this; this->next_node(); return copy; }
        }; // class reverse_node_iterator
    public:
        basic_trie() { this->_root = std::make_shared<node>(); }
        ~basic_trie() {}

        basic_trie(basic_trie&& src) noexcept { this->_root = std::move(src._root); } // move constructing
        basic_trie& operator=(basic_trie&& src) noexcept { this->_root = std::move(src._root); return *this; } // move assignment
        basic_trie(const basic_trie& src) = delete; // disable copy constructing
        basic_trie& operator=(const basic_trie& src) = delete; // disable copy assignments

        bool has_node(const key_t& _key);
        std::shared_ptr<value_t>& at(const key_t& key);
        std::shared_ptr<value_t>& operator[] (const key_t& key);
        bool insert(const key_t& key, std::shared_ptr<value_t> value);
        trie::basic_trie<key_t, value_t> subtrie(const key_t& key);
        trie::basic_trie<key_t, value_t> clone();

        node_iterator node_begin() { return ++node_iterator(_root); }
        const node_iterator node_begin() const { return ++node_iterator(_root); }
        node_iterator node_end() { return node_iterator(_root); }
        const node_iterator node_end() const { return node_iterator(_root); }

        reverse_node_iterator node_rbegin() { return ++reverse_node_iterator(_root); }
        const reverse_node_iterator node_rbegin() const { return ++reverse_node_iterator(_root); }
        reverse_node_iterator node_rend() { return reverse_node_iterator(_root); }
        const reverse_node_iterator node_rend() const { return reverse_node_iterator(_root); }

    }; // class basic_trie    
} // namespace trie
