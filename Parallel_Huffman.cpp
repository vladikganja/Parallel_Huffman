#include "Parallel_Huffman.h"

void Parallel_Huffman::archieve() {

    // DIALOGUE WITH USER /////////////////////////////////////////////////////
    std::string input, output;
    uifstream input_file;
    uofstream output_file;

    while (true) {
        std::cout << "Enter the input file name\n";
        //std::cin >> input;
        input_file.open("../tests/t1.txt", std::ios::binary);
        if (!input_file.is_open()) {
            std::cout << "Not found\n";
            continue;
        }
        else {
            std::cout << "Found\n";
            break;
        }
    }

    while (true) {
        std::cout << "Enter the output file name\n";
        //std::cin >> output;
        output_file.open("../tests/td1.txt", std::ios::binary);

        if (!output_file.is_open()) {
            std::cout << "Not found\n";
            continue;
        }
        else {
            std::cout << "Found\n";
            break;
        }
    }

    std::string mode;
    std::cout << "Make effective archieving? (takes more time), (y/n): ";
    std::cin >> mode;
    /////////////////////////////////////////////////////////////////////////////

    auto open_start = std::chrono::high_resolution_clock::now();
    input_file.seekg(0, std::ios::end);
    size_t size = input_file.tellg();
    std::string s(size, '\0');
    input_file.seekg(0);
    ustring DATA(size, '\0');
    input_file.read(&DATA[0], size);
    auto open_end = std::chrono::high_resolution_clock::now();

    std::vector<ustring> PARALLEL_COMPRESSED_DATA;

    // ARCHIEVING /////////////////////////////////////////////////////////////
    std::cout << "====================Encoding started====================\n";
    auto enc_start = std::chrono::high_resolution_clock::now();

    if (mode == "y")
        PARALLEL_COMPRESSED_DATA = Archiever::effective_archieve(DATA);
    else
        PARALLEL_COMPRESSED_DATA = Archiever::simple_archieve(DATA);

    auto enc_end = std::chrono::high_resolution_clock::now();
    ///////////////////////////////////////////////////////////////////////////

    std::size_t total_read_bytes = DATA.size();
    std::size_t total_written_bytes = 0;

    for (auto& x : PARALLEL_COMPRESSED_DATA) {
        output_file << x;
        total_written_bytes += x.size();
    }

    std::chrono::duration<double> open_diff = open_end - open_start;
    std::chrono::duration<double> enc_diff = enc_end - enc_start;
    std::cout.precision(6);
    std::cout << GRN << "Successful archiving!\n";
    std::cout << std::fixed << WHT << "Bytes read: " << GRN << total_read_bytes << WHT << "\nBytes written: " << GRN << total_written_bytes <<
        WHT << "\nFile compression: " << GRN << static_cast<double>(total_read_bytes - total_written_bytes) * 100.0 / total_read_bytes << "%\n";

    std::cout << WHT << "Opening: " << GRN << open_diff.count() << " sec\n" << WHT;
    std::cout << WHT << "Archiving: " << GRN << enc_diff.count() << " sec\n" << WHT;
    std::cout << "====================Encoding ended====================\n\n";

    input_file.close();
    output_file.close();
}

void Parallel_Huffman::unzip() {
    Decoder::unzip();
}
