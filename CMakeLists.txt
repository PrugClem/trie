# CMakeList.txt : CMake project for trie, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("trie")

# Add source to this project's executable.
add_executable (trie "trie.cpp" "trie.hpp" "trie/trie.hpp" "trie/key.hpp" "trie/impl/key256_impl.hpp" "trie/impl/key16_impl.hpp" "trie/basic_trie.hpp" "trie/impl/basic_trie_impl.hpp" "trie/impl/basic_node_iterator_impl.hpp" "trie/impl/basic_value_iterator_impl.hpp")

# TODO: Add tests and install targets if needed.
