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
    constexpr size_t children_count = 256;
    struct key {
        std::vector<uint8_t> _key;

        key() {}
        key(void* data, size_t len)
        {
            _key.resize(len);
            for (size_t i = 0; i < len; i++)
            {
                _key.at(i) = ((uint8_t*)data)[i]; // copy every element from the source into the key
            }
        }
        key(const std::string& string_key)
        {
            _key.resize(string_key.length());
            for (size_t i = 0; i < string_key.size(); i++)
            {
                _key.at(i) = string_key.at(i); // copy every ASCII character into the key
            }

        }
        uint8_t get_element(size_t index) const { return _key.at(index); } // get a specific byte of the key
        std::size_t size() const { return this->_key.size(); }
        void push_back(uint8_t data) { this->_key.push_back(data); }
        void pop_back() { this->_key.pop_back(); }
        void clear() { this->_key.clear(); }
    }; // struct key

    std::ostream& operator<<(std::ostream& o, const ::trie::key& _key)
    {
        for (std::size_t i = 0; i < _key.size(); i++)
            o << _key.get_element(i);
        return o;
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
             *  \brief  returns a std::shred_ptr to the parent node.
             *          Note that the returned shared_ptr might cause a memory leak if it is not cleaned up.
             *          Note that the return might be a nullptr.
             */
            bool has_children()
            {
                return this->first_child() != nullptr;
            }
            std::shared_ptr<node> first_child()
            {
                for (size_t i = 0; i < children_count; i++)
                    if (this->children.at(i) != nullptr) return this->children.at(i);
                return nullptr;
            }
            /**
             *  \brief get a child shared_ptr
             */
            std::shared_ptr<node>& get_child(std::uint8_t key_element)
            {
                return this->children.at(key_element);
            }
        }; // struct node

        std::shared_ptr<node> _root;

        void print_trienode(std::ostream& out, std::shared_ptr<::trie::trie<T>::node> node, std::size_t level)
        {
            for(std::size_t i=0; i<children_count; i++)
            {
                std::shared_ptr<::trie::trie<T>::node> child = node->get_child(i);
                if (child != nullptr)
                {
                    for (size_t l = 0; l < level; l++)
                    {
                        out << " ";
                    }
                    out << (char)i;
                    if (child->data != nullptr) out << "  : " << *(child->data);
                    out << std::endl;
                    print_trienode(out, child, level+1);
                }
            }
        }

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
        class basic_node_iterator
        {
        protected:
            // protected internal pointers to allow inherited classes to access them
            std::shared_ptr<node> root_node;
            std::shared_ptr<node> cur_node;
            ::trie::key cur_key;
            std::size_t child_key = 0;
        public: // public constructors and destructors
            basic_node_iterator(std::shared_ptr<node> root_node, std::shared_ptr<node> init_node, ::trie::key init_key) :
                root_node(root_node), cur_node(init_node), cur_key(init_key) {}
            basic_node_iterator() {}
            virtual ~basic_node_iterator() {}
        protected:
            void step_forward() // helper function to step the iterator one step forward
            {
                std::shared_ptr<node> child;
                while (true) { // the loop is neccessary for the code to function properly, it will return eventually unless the trie is malformed
                    child = nullptr;
                    
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
            void step_backwards()
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
        }; // class basic_node_iterator

        class node_iterator : public basic_node_iterator
        {
        public:
            node_iterator(std::shared_ptr<node> root_node, std::shared_ptr<node> init_node, ::trie::key init_key) :
                basic_node_iterator(root_node, init_node, init_key) {}
            node_iterator() : basic_node_iterator() {}
            ~node_iterator() {}

            node_iterator& operator++()
            {
                this->step_forward();
                return *this;
            }
            node_iterator operator++(int)
            {
                node_iterator copy = *this;
                this->step_forward();
                return copy;
            }
            node_iterator& operator--()
            {
                this->step_backwards();
                return *this;
            }
            node_iterator operator--(int)
            {
                node_iterator copy = *this;
                this->step_backwards();
                return copy;
            }
            bool operator==(const node_iterator& other)
            {
                if (this->root_node != other.root_node) throw std::out_of_range("Iterators are not obtained from the same trie!");
                return this->cur_node == other.cur_node;
            }
            bool operator!=(const node_iterator& other)
            {
                return !(*this == other);
            }

            operator bool()
            {
                return this->cur_node != nullptr;
            }
            bool operator!()
            {
                return this->cur_node == nullptr;
            }
            ::trie::key get_key() { return this->cur_key; }
            std::shared_ptr<T> get_data() { return this->cur_node->data; }
        }; // class node_iterator

        trie() { this->_root = std::make_shared<node>(); }
        ~trie() {}

        trie(trie&& src) noexcept { this->_root = std::move(src._root); } // move constructing
        trie& operator=(trie&& src) noexcept { this->_root = std::move(src._root); return *this; } // move assignment
        [[deprecated("Use the clone method")]] trie(const trie& src) = delete; // copy constructing
        [[deprecated("Use the clone method")]] trie& operator=(const trie& src) = delete; // copy assignment

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

        ::trie::trie<T>&& subtrie(::trie::key _key)
        {
            std::shared_ptr<node> newroot = this->get_node(_key);
            if(newroot == nullptr) throw std::out_of_range("Trie does not have the requested child");
            return std::move(::trie::trie<T>(newroot) );
        }

        node_iterator node_begin()
        {
            return ++node_iterator(_root, nullptr, ::trie::key());
        }
        node_iterator node_end()
        {
            return node_iterator(_root, nullptr, ::trie::key());
        }

        void print_trie(std::ostream& out)
        {
            print_trienode(out, _root, 0);
        }
    }; // class trie
}; // namespace trie

// TODO: Reference additional headers your program requires here.
