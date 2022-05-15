#pragma once
#include <iostream>
#include <vector>
#include "Utils.h"

class decode_node {
public:
    explicit decode_node();
    decode_node* son_l;
    decode_node* son_r;
    decode_node* parent;
    unsigned char ch;
};

class Decoder {
public:
    static void unzip();
private:
    static const std::vector<decode_node>& restore_tree(ustring inzip_buffer, std::size_t hash_size);
};

