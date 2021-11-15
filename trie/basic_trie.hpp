/**
* @file     trie/basic_trie.hpp
* @brief    main include file for the trie data structure class
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
    /**
     * @brief the main trie class. This class is a assiciative container mapping from a byte sequence to a pointer. 
     *  The byte sequence is managed by the trie key class and the pointer is managed by the std::shared_ptr
     *  The children count defines how many children one node can have, this value must be one of the following: 2/4/16/256
     *  The value_t defines the datatype stored by the std::shared_ptr's
     * 
     * @tparam children_count 
     * @tparam value_t 
     */
    template<std::size_t children_count, typename value_t>
    class basic_trie
    {
    public:
        using key_t = trie::basic_key<children_count>;
    protected:
        struct node
        {
            std::array<std::shared_ptr<node>, children_count> children;
            std::shared_ptr<value_t> data;

            /**
             * @brief get the node's child with the index key_element
             * @throws std::out_of_range from std::vector::at if the parameter is bigger than the number of children supported by the trie
             */
            std::shared_ptr<node>& child(std::size_t key_element);
        };

        std::shared_ptr<node> _root;

        /**
         * @brief Get a pointer pointing tho the node at the key _key, nullptr if the node does not exist
         * @param _key which node to get
         * @return nullptr if the node is not present in the trie, pointer to the node otherwise
         */
        std::shared_ptr<node> get_node(const key_t& _key);
        /**
         * @brief Get a pointer pointing to the node at key _key, create that node (and all required parent nodes) if the node does not exists
         * 
         * @param _key which node to add to the trie
         * @return pointer pointing to the node, if this is a nullptr, something went really wrong
         * @throws std::bad_alloc from std::make_shared()
         */
        std::shared_ptr<node> add_node(const key_t& _key);
        /**
         * @brief unlink a node and all its children nodes from the trie, after this method call the node will no longer exist
         * 
         * @param _key which node to unlink
         * @return true to indicate that the node was unlinked, false to indicate that the node did not exist
         */
        bool unlink_node(const key_t& _key);

        /**
         * @brief internal helper constructor
         */
        basic_trie(std::shared_ptr<node> root) { this->_root = root; }

    public:
        basic_trie() { this->clear(); }
        ~basic_trie() {}

        basic_trie(basic_trie&& src) noexcept { this->_root = std::move(src._root); } // move constructing
        basic_trie& operator=(basic_trie&& src) noexcept { this->_root = std::move(src._root); return *this; } // move assignment
        basic_trie(const basic_trie& src) = delete; // disable copy constructing
        basic_trie& operator=(const basic_trie& src) = delete; // disable copy assignments

        /**
         * @brief check if the trie has a specific node
         * 
         * @param _key the key to check
         * @return true if the trie has the requested node, false otherwise
         */
        bool has_node(const key_t& _key);
        /**
         * @brief get the pointer to the value stored at a key, this pointer can be a nullptr!
         * 
         * @param key the key to the value
         * @return reference to a std::shared_ptr of value type
         * @throws std::out_of_range if the trie does not have the requested node
         */
        std::shared_ptr<value_t>& at(const key_t& key);
        /**
         * @brief get or create the pointer to the value stored at a key, this pointer can be a nullptr!
         * 
         * @param key the key to the value
         * @return reference to a std::shared_ptr of value type
         */
        std::shared_ptr<value_t>& operator[] (const key_t& key);
        /**
         * @brief insert a pointer to a value into the trie
         * 
         * @param key the key where to store the value
         * @param value the pointer to the value that should be stored
         * @return true if the value could be inserted, false if the value already stored data
         */
        bool insert(const key_t& key, std::shared_ptr<value_t> value);
        /**
         * @brief erade a node and all of it's children nodes
         * 
         * @param key the key of the node that should be removed
         * @return true to indicate that a node has been removed, false if no node was present
         */
        bool erase(const key_t& key);
        /**
         * @brief extract all values from the source trie that are not present in this trie. If a key has a node in both tries, it is not removed from the source trie
         *  INFO: nodes are moved from source into this, so the source trie will most likely be modified
         * 
         * @param source which trie to merge into this
         */
        void merge(::trie::basic_trie<children_count, value_t>& source);
        /**
         * @brief get a subtrie of this trie. INFO: the subtrie is NOT a copy of this trie, all modifications of the subtrie will also modify this trie!
         * 
         * @param key where to set the root of the subtrie
         * @return new instance of the trie class having a root node from this trie
         */
        trie::basic_trie<children_count, value_t> subtrie(const key_t& key);
        /**
         * @brief clones all nodes from this trie and creates a copy of this trie
         * 
         * @return new instance of the trie class hving a copy of all nodes
         */
        trie::basic_trie<children_count, value_t> clone();
        /**
         * @brief removes all nodes from the trie and resets it into the valid empty state
         */
        void clear();
        /**
         * @brief returns the number of elements stored in the trie, INFO: this method currently has a complexity of O(n)
         */
        std::size_t size();

    protected:
        /**
         * @brief the basic_node_iterator is the base for all of the trie's iterators.
         *  It provides basic comparison functionality and a way to get the current iterator key and data pointer
         *  This class also implements the basic iterating mechanic, which iterates over the trie in a circular way.
         */
        struct basic_node_iterator
        {
        protected:
            std::shared_ptr<node> root_node{ nullptr };
            mutable std::shared_ptr<node> cur_node{ nullptr };
            mutable key_t cur_key;
            mutable std::ptrdiff_t child_element{ 0 };

        public:
            basic_node_iterator() {}
            basic_node_iterator(std::shared_ptr<node> root_node) : root_node(root_node) {}
            virtual ~basic_node_iterator() {}

            inline bool operator==(const basic_node_iterator& other) const;
            inline bool operator!=(const basic_node_iterator& other) const { return !(*this == other); }

            /**
             * @brief check if the iterator is currently in the 'null' (invalid / outside) node
             */
            inline bool is_null() const { return this->cur_node == nullptr; }
            inline operator bool() const { return !this->is_null(); }
            inline bool operator!() const { return this->is_null(); }

            /**
             * @brief Get the key of where the iterator is currently at
             */
            inline key_t get_key() { return this->cur_key; }
            /**
             * @brief Get the key of where the iterator is currently at
             */
            inline const key_t& get_key() const { return this->cur_key; }
            /**
             * @brief Get the data pointer of where the iterator is currently at
             */
            inline std::shared_ptr<value_t>& get_data() { return this->cur_node->data; }
            /**
             * @brief Get the data pointer of where the iterator is currently at
             */
            inline const std::shared_ptr<value_t>& get_data() const { return this->cur_node->data; }

        protected:
            void next_node() const;
            void prev_node() const;
        }; // struct basic_node_iterator

        struct basic_value_iterator : public basic_node_iterator
        {
            basic_value_iterator() : basic_node_iterator() {}
            basic_value_iterator(std::shared_ptr<node> root_node) : basic_node_iterator(root_node) {}
            virtual ~basic_value_iterator() {}
        protected:
            void next_value() const;
            void prev_value() const;
        }; // struct basic_value_iterator

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

            const reverse_node_iterator& operator++() const { this->prev_node(); return *this; }
            const reverse_node_iterator operator++(int) const { reverse_node_iterator copy = *this; this->prev_node(); return copy; }
            const reverse_node_iterator& operator--() const { this->next_node(); return *this; }
            const reverse_node_iterator operator--(int) const { reverse_node_iterator copy = *this; this->next_node(); return copy; }
        }; // class reverse_node_iterator

        class value_iterator : public basic_value_iterator
        {
        public:
            value_iterator() {}
            value_iterator(std::shared_ptr<node> root_node) : basic_value_iterator(root_node) {}
            ~value_iterator() {}

            const value_iterator& operator++() const { this->next_value(); return *this; }
            const value_iterator operator++(int) const { value_iterator copy = *this; this->next_value(); return copy; }
            const value_iterator& operator--() const { this->prev_value(); return *this; }
            const value_iterator operator--(int) const { value_iterator copy = *this; this->prev_value(); return copy; }
        }; // class value_iterator

        class reverse_value_iterator : public  basic_value_iterator
        {
        public:
            reverse_value_iterator() {}
            reverse_value_iterator(std::shared_ptr<node> root_node) : basic_value_iterator(root_node) {}
            ~reverse_value_iterator() {}

            const reverse_value_iterator& operator++() const { this->prev_value(); return *this; }
            const reverse_value_iterator operator++(int) const { reverse_value_iterator copy = *this; this->prev_value(); return copy; }
            const reverse_value_iterator& operator--() const { this->next_value(); return*this; }
            const reverse_value_iterator operator--(int) const { reverse_value_iterator copy = *this; this->next_value(); return copy; }
        }; // class reverse_value_iterator
    public:
        node_iterator node_begin() { return ++node_iterator(_root); }
        const node_iterator node_begin() const { return ++node_iterator(_root); }
        node_iterator node_end() { return node_iterator(_root); }
        const node_iterator node_end() const { return node_iterator(_root); }

        reverse_node_iterator node_rbegin() { return ++reverse_node_iterator(_root); }
        const reverse_node_iterator node_rbegin() const { return ++reverse_node_iterator(_root); }
        reverse_node_iterator node_rend() { return reverse_node_iterator(_root); }
        const reverse_node_iterator node_rend() const { return reverse_node_iterator(_root); }

        value_iterator begin() { return ++value_iterator(_root); }
        const value_iterator begin() const { return ++value_iterator(_root); }
        value_iterator end() { return value_iterator(_root); }
        const value_iterator end() const { return value_iterator(_root); }

        reverse_value_iterator rbegin() { return ++reverse_value_iterator(_root); }
        const reverse_value_iterator rbegin() const { return ++reverse_value_iterator(_root); }
        reverse_value_iterator rend() { return reverse_value_iterator(_root); }
        const reverse_value_iterator rend() const { return reverse_value_iterator(_root); }
    }; // class basic_trie
} // namespace trie
