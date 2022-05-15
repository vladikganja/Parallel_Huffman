#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include "Utils.h"

class encode_node {
public:
    explicit encode_node(int _weight, unsigned char _ch);
    encode_node* son_l;
    encode_node* son_r;
    encode_node* parent;
    int weight;
    unsigned char ch;
    bool symb;
    bool used;
};

class Archiever {
public:
    static ustring simple_archieve(const ustring& DATA);
    static ustring effective_archieve(const ustring& DATA);

private:
    static std::vector<ustring> parallel_compression(const ustring& DATA, const std::size_t BLOCK_SIZE = 64);
    static std::vector<ustring> list_to_tree(encode_node* root, ustring& hash, std::size_t& size);
    static bool comparator(encode_node* n1, encode_node* n2);
    static encode_node* table_to_list(const std::vector<int>& counts_table);
    static std::vector<int> data_to_table(const ustring& DATA, std::size_t block_size, std::size_t offset);
};

