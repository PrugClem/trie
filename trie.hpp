/**
* @file     trie.hpp
* @brief    main include file for the header-only trie implementation
* @author   Clemens Pruggmayer
* (c) 2021 by Clemens Pruggmayer
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <memory>
#include <array>
#include <vector>
#include <string>
#include <stdexcept>
#include <ostream>

namespace trie {
// allowed values are: 16, 256; 16 is the mainly tested version
#define __trie_children_count__ 16
    constexpr const std::size_t children_count = __trie_children_count__;

    struct key {
        std::vector<uint8_t> _key;
        std::size_t _size = 0;

        key()
        {
            this->_key.clear();
            this->_size = 0;
        }
        key(void* data, std::size_t len)
        {
            _key.resize(len);
            _size = len * 2;
            for (std::size_t i = 0; i < len; i++)
            {
                _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
            }
        }
        key(const std::string& string_key)
        {
            _key.resize(string_key.length());
            _size = string_key.length() * 2;
            for (std::size_t i = 0; i < string_key.size(); i++)
            {
                _key.at(i) = string_key.at(i); // copy every ASCII character into the key
            }

        }
        std::string to_string() const
        {
            std::string result;
            for (std::size_t i = 0; i < _key.size(); i++)
            {
                result.push_back(_key.at(i));
            }
            return result;
        }
        std::string to_hex_string() const
        {
            static constexpr const char* __4b_int_to_hex_char = "0123456789ABCDEF";
            std::string result("0x");
            for(std::size_t i=0; i< this->size(); i++)
            {
                result.push_back(__4b_int_to_hex_char[this->get_element(i) & 0xF]);
            }
            return result;
        }
#if __trie_children_count__ == 256
// code using a 256-trie
        uint8_t get_element(std::size_t index) const { return _key.at(index); } // get a specific byte of the key
        std::size_t size() const { return this->_key.size(); }
        void push_back(uint8_t data) { this->_key.push_back(data); }
        void pop_back() { this->_key.pop_back(); }
        void clear() { this->_key.clear(); }
#elif __trie_children_count__ == 16
// code using a 16-trie
        friend std::ostream& operator<<(std::ostream& o, const ::trie::key& key);
        uint8_t get_element(std::size_t index) const
        {
            // get raw binary data from key bytes
            uint8_t data = this->_key.at(index >> 1);
            if (index % 2 == 0)
            {
                // even, use lower bits
                data = data & 0xF;
            }
            else
            {
                // odd, use higher bits
                data = (data & 0xF0) >> 4;
            }
            return data; // the value can only hold values in the range 0x0 to 0xF because of the code above
        }
        std::size_t size() const { return this->_size; }
        void push_back(uint8_t data)
        {
            if (this->_size % 2 == 0)
            {
                // even, new byte needed
                this->_key.push_back(data & 0xF);
                this->_size++;
            }
            else
            {
                // odd, data needs to be stffed into the last byte
                this->_key.at(this->_key.size()-1) |= ((data << 4) & 0xF0);
                this->_size++;
            }
        }
        void pop_back()
        {
            if (this->_size % 2 == 0)
            {
                // even, removal does not remove a byte, but the last 4 bits are cleared
                this->_key.at(this->_key.size() - 1) &= 0xF;
                this->_size--;
            }
            else
            {
                // odd, removal will remove the last byte, clearing is not required since the byte is discarded
                this->_key.pop_back();
                this->_size--;
            }
        }
        void clear()
        {
            this->_key.clear();
            this->_size = 0;
        }
#else
# error Unsupported children count, the supported values are 16 and 256
#endif
    }; // struct key

    [[deprecated("convert the key to a string brefore printing")]] std::ostream& operator<<(std::ostream& o, const ::trie::key& _key)
    {
        return o << _key.to_hex_string();
    }

    template<typename T>
    class trie
    {
    protected:
        struct node {
            std::array<std::shared_ptr<node>, children_count> children;
            //std::weak_ptr<node> parent;
            std::shared_ptr<T> data;

            /**
             *  \brief get a child shared_ptr
             */
            std::shared_ptr<node>& get_child(std::uint8_t key_element)
            {
                return this->children.at(key_element);
            }
        }; // struct node

        std::shared_ptr<node> _root;

        std::shared_ptr<node> get_node(const ::trie::key& _key)
        {
            std::shared_ptr<node> helper = _root, child;
            for (std::size_t i = 0; i < _key.size(); i++)
            {
                child = helper->get_child(_key.get_element(i)); // get the child item
                if (child == nullptr) return nullptr;
                helper = child; // set the helper pointer to the child and proceed to the next key element
            }
            return helper;
        }

        std::shared_ptr<node> add_node(const ::trie::key& _key)
        {
            std::shared_ptr<node> helper = _root, child;
            // search for the requested child node and create any nodes that are missing
            for (std::size_t i = 0; i < _key.size(); i++)
            {
                child = helper->get_child(_key.get_element(i)); // get the child item
                if (child == nullptr)
                {
                    child = helper->get_child(_key.get_element(i)) = std::make_shared<node>(); // if the child item is empty, allocate a new one and set the helper pointer to it
                }
                helper = child;
            }
            return helper;
        }

        trie(std::shared_ptr<node> root) { this->_root = root; }

    public:
        static constexpr std::size_t node_size = sizeof(node);
        class basic_node_iterator
        {
        protected:
            // protected internal pointers to allow inherited classes to access them
            std::shared_ptr<node> root_node;
            mutable std::shared_ptr<node> cur_node;
            mutable ::trie::key cur_key;
            mutable std::size_t child_key = 0;
        public: // public constructors and destructors
            basic_node_iterator(std::shared_ptr<node> root_node, std::shared_ptr<node> init_node, ::trie::key init_key) :
                root_node(root_node), cur_node(init_node), cur_key(init_key) {}
            basic_node_iterator() {}
            virtual ~basic_node_iterator() {}
        protected:
            void step_forward() const // helper function to step the iterator one step forward
            {
                while (true) { // the loop is neccessary for the code to function properly, it will return eventually unless the trie is malformed
                    
                    // if the current node is the null node, go to the root node
                    if (cur_node == nullptr)
                    {
                        cur_node = root_node;
                        cur_key.clear();
                        child_key = 0;
                        return;
                    }
                    // find the next child
                    for (std::size_t i = child_key; i < children_count; i++)
                    {
                        if (cur_node->get_child(uint8_t(i)) != nullptr)
                        {
                            // if a child was found, go to child node and stop iterating
                            cur_node = cur_node->get_child(uint8_t(i));
                            cur_key.push_back(uint8_t(i));
                            child_key = 0;
                            return;
                        }
                    }
                    // no child found, go to parent ( which is the null ode for the root node )
                    if (cur_node == root_node)
                    {
                        cur_node = nullptr;
                        cur_key.clear();
                        child_key = 0;
                        return;
                    }
                    child_key = (std::size_t)cur_key.get_element(cur_key.size() - 1) + 1;
                    cur_key.pop_back();
                    cur_node = ::trie::trie<T>(root_node).get_node(cur_key);
                }
            }
            void step_backward() const
            {
                if(cur_node == nullptr)
                {
                    cur_node = root_node;
                    cur_key.clear();
                    child_key = children_count-1;
                }
                else if (cur_node == root_node)
                {
                    cur_node = nullptr;
                    cur_key.clear();
                    child_key = 0;
                    return;
                } 
                else 
                {
                    child_key = (std::size_t)cur_key.get_element(cur_key.size() - 1) - 1;
                    cur_key.pop_back();
                    cur_node = ::trie::trie<T>(root_node).get_node(cur_key);
                }

                while (true)
                {
                    for (std::ptrdiff_t i = child_key; i >= 0; i--)
                    {
                        if (cur_node->get_child((uint8_t)i) != nullptr)
                        {
                            cur_node = cur_node->get_child((uint8_t)i);
                            cur_key.push_back((uint8_t)i);
                            child_key = children_count-1;
                            i = child_key;
                        }
                    }
                    return;
                }
            }
        public:
            bool operator==(const basic_node_iterator& other) const
            {
                if (this->root_node != other.root_node) throw std::out_of_range("Iterators are not obtained from the same trie!");
                return this->cur_node == other.cur_node;
            }
            bool operator!=(const basic_node_iterator& other) const
            {
                if (this->root_node != other.root_node) throw std::out_of_range("Iterators are not obtained from the same trie!");
                return !(*this == other);
            }

            operator bool() const { return this->cur_node != nullptr; }
            bool operator!() const { return this->cur_node == nullptr; }

            const ::trie::key get_key() const { return this->cur_key; }
            std::shared_ptr<T> get_data() { return this->cur_node->data; }
            const std::shared_ptr<T> get_data() const { return this->cur_node->data; }
        }; // class basic_node_iterator

        class node_iterator : public basic_node_iterator
        {
        public:
            node_iterator(std::shared_ptr<node> root_node, std::shared_ptr<node> init_node, ::trie::key init_key) :
                basic_node_iterator(root_node, init_node, init_key) {}
            node_iterator() : basic_node_iterator() {}
            ~node_iterator() {}

            const node_iterator& operator++() const { this->step_forward(); return *this; }
            const node_iterator operator++(int) const { node_iterator copy = *this; this->step_forward(); return copy; }
            const node_iterator& operator--() const { this->step_backward(); return *this; }
            const node_iterator operator--(int) const { node_iterator copy = *this; this->step_backward(); return copy; }
        }; // class node_iterator

        class reverse_node_iterator : public basic_node_iterator
        {
        public:
            reverse_node_iterator(std::shared_ptr<node> root_node, std::shared_ptr<node> init_node, ::trie::key init_key) :
                basic_node_iterator(root_node, init_node, init_key) {}
            reverse_node_iterator() : basic_node_iterator() {}
            ~reverse_node_iterator() {}

            const reverse_node_iterator& operator++() const { this->step_backward(); return *this; }
            const reverse_node_iterator operator++(int) const { reverse_node_iterator copy = *this; this->step_backward(); return copy; }
            const reverse_node_iterator& operator--() const { this->step_forward(); return *this; }
            const reverse_node_iterator operator--(int) const { reverse_node_iterator copy = *this; this->step_forward(); return copy; }
        }; // class reverse_node_iterator

        trie() { this->_root = std::make_shared<node>(); }
        ~trie() {}

        trie(trie&& src) noexcept { this->_root = std::move(src._root); } // move constructing
        trie& operator=(trie&& src) noexcept { this->_root = std::move(src._root); return *this; } // move assignment
#if defined TRIE_ALLOW_DEPRECATED_COPY_CLONING
        [[deprecated("Use the clone method")]] trie(const trie& src) { *this = src.clone(); } // copy constructing
        [[deprecated("Use the clone method")]] trie& operator=(const trie& src) { *this = src.clone(); return *this; } // copy assignment
#else
        trie(const trie& src) = delete; // disable copy constructing
        trie& operator=(const trie& src) = delete; // disable copy assignments
#endif

        bool has_node(const ::trie::key& _key)
        {
            return this->get_node(_key) != nullptr;
        }

        std::shared_ptr<T>& at(const ::trie::key& _key)
        {
            std::shared_ptr<node> _node = this->get_node(_key); // get the node containing the child
            if(_node == nullptr) throw std::out_of_range("Trie does not have the requested child"); // throw exception if child does not exists
            return _node->data; // return data
        }

        std::shared_ptr<T>& operator[](const ::trie::key& _key)
        {
            std::shared_ptr<node> _node = this->add_node(_key); // get / add the node containing the child
            return _node->data; // return data
        }

        bool insert(const ::trie::key& _key, std::shared_ptr<T> value)
        {
            std::shared_ptr<node> helper = this->add_node(_key);
            if (helper->data == nullptr)
            {
                // if the node does not contain data, set the data and return true
                helper->data = value;
                return true;
            }
            else
            {
                // if the child already contained data, return false
                return false;
            }
        }

        ::trie::trie<T> subtrie(::trie::key _key)
        {
            std::shared_ptr<node> newroot = this->get_node(_key);
            if(newroot == nullptr) throw std::out_of_range("Trie does not have the requested child");
            return ::trie::trie<T>(newroot);
        }
        ::trie::trie<T> clone() const
        {
            ::trie::trie<T> _clone;
            for (auto iter = this->node_begin(); iter != this->node_end(); iter++)
                _clone.insert(iter.get_key(), iter.get_data());
            return ::trie::trie<T>(_clone._root);
        }

        node_iterator node_begin() { return ++node_iterator(_root, nullptr, ::trie::key()); }
        const node_iterator node_begin() const { return ++node_iterator(_root, nullptr, ::trie::key()); }
        node_iterator node_end() { return node_iterator(_root, nullptr, ::trie::key()); }
        const node_iterator node_end() const { return node_iterator(_root, nullptr, ::trie::key()); }

        reverse_node_iterator node_rbegin() { return ++reverse_node_iterator(_root, nullptr, ::trie::key()); }
        const reverse_node_iterator node_rbegin() const { return ++reverse_node_iterator(_root, nullptr, ::trie::key()); }
        reverse_node_iterator node_rend() { return reverse_node_iterator(_root, nullptr, ::trie::key()); }
        const reverse_node_iterator node_rend() const { return reverse_node_iterator(_root, nullptr, ::trie::key()); }
    }; // class trie
}; // namespace trie

