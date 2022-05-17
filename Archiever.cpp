#include "Archiever.h"

encode_node::encode_node(int _weight, unsigned char _ch) : weight(_weight), ch(_ch) {
    son_l = nullptr;
    son_r = nullptr;
    parent = nullptr;
    symb = true;
    used = false;
}

// ARCHIEVING /////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<ustring> Archiever::simple_archieve(const ustring& DATA) {
    return parallel_compression(DATA);
}

std::vector<ustring> Archiever::effective_archieve(const ustring& DATA) {
    return std::vector<ustring>();
}

// IMPL ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<ustring> Archiever::parallel_compression(const ustring& DATA, const std::size_t BLOCK_SIZE) {

    std::vector<ustring> _chunks_(static_cast<std::size_t>(DATA.size() / (BLOCK_SIZE * 1024)) + 1,
                         ustring(Utils::META_RESERVED, '\0'));
    _chunks_[0][0] = static_cast<unsigned char>(BLOCK_SIZE & 255);
    _chunks_[0].resize(Utils::META_RESERVED + 1);
    std::size_t threads_amount = std::thread::hardware_concurrency();
    std::vector<std::thread> _threads_(threads_amount);
    std::mutex _mtx_;

    std::size_t thread = 0;

    auto calc = [&]() -> void {
        _mtx_.lock();
        thread_local std::size_t _thread = thread;
        ++thread;
        _mtx_.unlock();
        for (std::size_t chunk = _thread; chunk * BLOCK_SIZE * 1024 < DATA.size(); chunk += threads_amount) {
            std::size_t offset = chunk * BLOCK_SIZE * 1024;
            if (offset > DATA.size())
                break;
            std::size_t block_size = std::min(BLOCK_SIZE * 1024, DATA.size() - offset);
            std::vector<int> counts_table = data_to_table(DATA, block_size, offset);
            encode_node* root = table_to_list(counts_table);
            std::size_t size = 0;
            ustring hash(320, '\0');
            std::vector<ustring> codes_table = list_to_tree(root, hash, size);

            _chunks_[chunk].reserve(BLOCK_SIZE * 1024 * 2);

            for (std::size_t i = 0; i < size; ++i)
                _chunks_[chunk].push_back(hash[i]);

            unsigned char buf = '\0';
            int count = 0;
            for (std::size_t i = 0; i < block_size; ++i) {
                unsigned char c = DATA[i + offset];
                ustring tmp = codes_table[c];
                for (std::size_t j = 0; j < tmp.size(); ++j) {
                    int tmpi = static_cast<int>(tmp[j] - 48);
                    buf |= (tmpi << (7 - count++));
                    if (count == 8) {
                        _chunks_[chunk].push_back(buf);
                        count = 0;
                        buf = '\0';
                    }
                }
            }
            _chunks_[chunk].push_back(buf);

            // META FILLING ///////////////////////////////////
            std::size_t fbofs = static_cast<std::size_t>(!chunk); // first bit offset
            _chunks_[chunk][0 + fbofs] = static_cast<unsigned char>(size >> 8);
            _chunks_[chunk][1 + fbofs] = static_cast<unsigned char>(size & 255);
            _chunks_[chunk][2 + fbofs] = static_cast<unsigned char>((_chunks_[chunk].size() - Utils::META_RESERVED - size) >> 16);
            _chunks_[chunk][3 + fbofs] = static_cast<unsigned char>((_chunks_[chunk].size() - Utils::META_RESERVED - size) >> 8);
            _chunks_[chunk][4 + fbofs] = static_cast<unsigned char>((_chunks_[chunk].size() - Utils::META_RESERVED - size) & 255);
            _chunks_[chunk][5 + fbofs] = static_cast<unsigned char>(count);
            ///////////////////////////////////////////////////
        }
    };

    for (int i = 0; i < threads_amount; ++i) {
        _threads_[i] = std::thread(calc);
    }

    for (int thread = 0; thread < threads_amount; ++thread) {
        _threads_[thread].join();
    }

    return _chunks_;
}

std::vector<ustring> Archiever::list_to_tree(encode_node* root, ustring& hash, std::size_t& size) {
    int count = 0;
    unsigned char buf = '\0';
    ustring code;
    std::vector<ustring> table(256);

    encode_node* cur_root = root;
    while (true) {
        if (cur_root->son_l != nullptr && cur_root->son_l->used != true) {
            code.push_back('0');

            buf |= (0 << (7 - count++));
            if (count == 8) {
                hash[size++] = buf;
                count = 0;
                buf = '\0';
            }
            cur_root = cur_root->son_l;
            cur_root->used = true;
        }
        else if (cur_root->son_r != nullptr && cur_root->son_r->used != true) {
            code.push_back('1');
            cur_root->used = true;
            cur_root = cur_root->son_r;
        }
        else {
            if (cur_root->symb == true) {
                table[cur_root->ch] = code;
                buf |= (1 << (7 - count++));

                if (count == 8) {
                    hash[size++] = buf;
                    count = 0;
                    buf = '\0';
                }
                unsigned char letter = cur_root->ch;
                for (int i = 0; i < 8; ++i) {
                    int bit = 1 & letter >> (7 - i);

                    buf |= (bit << (7 - count++));
                    if (count == 8) {
                        hash[size++] = buf;
                        count = 0;
                        buf = '\0';
                    }
                }
            }
            cur_root->used = true;
            cur_root = cur_root->parent;
            if (cur_root == nullptr)
                break;
            code.pop_back();
        }
    }
    hash[size++] = buf;
    return table;
}

bool Archiever::comparator(encode_node* n1, encode_node* n2) {
    return n1->weight < n2->weight;
}

encode_node* Archiever::table_to_list(const std::vector<int>& counts_table) {
    std::list<encode_node*> list;
    for (int i = 0; i < 256; ++i) {
        if (counts_table[i])
            list.push_back(new encode_node(counts_table[i], i));
    }

    while (list.size() != 1) {
        list.sort(comparator);
        encode_node* s1 = *list.begin();
        list.pop_front();
        encode_node* s2 = *list.begin();
        list.pop_front();

        encode_node* parent = new encode_node(s1->weight + s2->weight, ' ');
        parent->son_l = s1;
        parent->son_r = s2;
        s1->parent = parent;
        s2->parent = parent;
        parent->symb = false;
        list.push_back(parent);
    }

    return list.front();
}

std::vector<int> Archiever::data_to_table(const ustring& DATA, std::size_t block_size, std::size_t offset) {
    std::vector<int> counts_table(256);
    for (std::size_t i = offset; i < offset + block_size; ++i)
        ++counts_table[DATA[i]];
    return counts_table;
}
