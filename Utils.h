#pragma once
#include <string>

constexpr auto RED = "\x1B[31m";
constexpr auto GRN = "\x1B[32m";
constexpr auto YEL = "\x1B[33m";
constexpr auto WHT = "\x1B[37m";

typedef std::basic_string<unsigned char> ustring;
typedef std::basic_stringstream<unsigned char> usstream;
typedef std::basic_ifstream<unsigned char> uifstream;
typedef std::basic_ofstream<unsigned char> uofstream;

class Utils {
public:
    static const int META_RESERVED = 6;
};

