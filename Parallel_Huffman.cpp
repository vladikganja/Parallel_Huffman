#include "Parallel_Huffman.h"

void Parallel_Huffman::archieve() {

    // DIALOGUE WITH USER /////////////////////////////////////////////////////
    std::string input, output;
    uifstream input_file;
    uofstream output_file;

    while (true) {
        std::cout << "Enter the input file name\n";
        std::cin >> input;
        input_file.open(input, std::ios::binary);
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
        std::cin >> output;
        output_file.open(output, std::ios::binary);

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

    usstream _str_stream_;
    _str_stream_ << input_file.rdbuf();
    ustring DATA = _str_stream_.str();
    ustring COMPRESSED_DATA;

    // ARCHIEVING WITH METRICS ////////////////////////////////////////////////
    std::cout << "====================Encoding started====================\n";
    auto start = std::chrono::high_resolution_clock::now();

    if (mode == "y")
        COMPRESSED_DATA = Archiever::effective_archieve(DATA);
    else
        COMPRESSED_DATA = Archiever::simple_archieve(DATA);

    auto end = std::chrono::high_resolution_clock::now();
    ///////////////////////////////////////////////////////////////////////////

    output_file << COMPRESSED_DATA;

    std::size_t total_read_bytes = DATA.size();
    std::size_t total_written_bytes = COMPRESSED_DATA.size();

    std::chrono::duration<double> diff = end - start;
    std::cout.precision(6);
    std::cout << GRN << "Successful archiving!\n";
    std::cout << std::fixed << WHT << "Bytes read: " << GRN << total_read_bytes << WHT << "\nBytes written: " << GRN << total_written_bytes <<
        WHT << "\nFile compression: " << GRN << static_cast<double>(total_read_bytes - total_written_bytes) * 100.0 / total_read_bytes << "%\n";

    std::cout << WHT << "Archiving: " << GRN << diff.count() << " sec\n" << WHT;
    std::cout << "====================Encoding ended====================\n\n";

    input_file.close();
    output_file.close();
}

void Parallel_Huffman::unzip() {
    Decoder::unzip();
}
