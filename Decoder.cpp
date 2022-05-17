#include "Decoder.h"

decode_node::decode_node() {
    son_l = nullptr;
    son_r = nullptr;
    parent = nullptr;
    ch = '\0';
}

void Decoder::unzip() {

}

const std::vector<decode_node> Decoder::restore_tree(ustring inzip_buffer, std::size_t hash_size) {
    int count = 0;
    int size = 0;

    std::vector<decode_node> root(512);
    decode_node* cur_root = &root[size++];
    for (int i = 0; i < hash_size;) {
        bool exit = false;
        if (cur_root == nullptr) {
            std::cerr << RED << "Decoding failed! Probably file was archived incorrectly.\n" << WHT;
            return {};
        }
        while (cur_root->son_l != nullptr && cur_root->son_r != nullptr) {
            cur_root = cur_root->parent;
            if (cur_root == nullptr) {
                exit = true;
                break;
            }
        }
        if (exit) {
            break;
        }

        unsigned char byte = inzip_buffer[i];
        bool b = 1 & (byte >> (7 - count++));
        if (count == 8) {
            ++i;
            count = 0;
        }

        if (b == 0) {
            if (cur_root->son_l != nullptr) {
                cur_root->son_r = &root[size++];
                cur_root->son_r->parent = cur_root;
                cur_root = cur_root->son_r;
            }
            cur_root->son_l = &root[size++];
            cur_root->son_l->parent = cur_root;
            cur_root = cur_root->son_l;
        }
        else {
            if (cur_root->son_l != nullptr) {
                cur_root->son_r = &root[size++];
                cur_root->son_r->parent = cur_root;
                cur_root = cur_root->son_r;
            }
            cur_root->ch = '\0';
            for (int k = 0; k < 8; ++k) {
                unsigned char tmp = inzip_buffer[i];
                int bit = 1 & tmp >> (7 - count++);
                cur_root->ch |= (bit << (7 - k));

                if (count == 8) {
                    ++i;
                    count = 0;
                }
            }
            cur_root = cur_root->parent;
        }
    }
    return root;
}
