#include "Cryptography.h"

#include <climits>
#include <cstdlib>

std::string Encrypt(std::string& input) {

    if (input.c_str() == NULL)
        return 0;

    size_t length = input.length();
    std::string binary;
    binary[0] = '\0';

    for (size_t i = 0; i < length; ++i) {
        srand(i);
        char ch = input[i];
        for (int j = 7; j >= 0; --j) {
            if (ch & (1 << j)) {
                binary += std::bitset<1>(rand()).to_string();
            }
            else {
                binary += std::bitset<1>(rand()).flip().to_string();
            }
        }
    }

    std::string output;
    std::istringstream in(binary);
    std::bitset<8> bitchar;

    long long int count = 1;

    while (in >> bitchar)
    {
        output += bitchar.flip().to_string();
        if (((count*8) % 120) == 0)
            output += "\n";
        count++;
    }

    return output;
}

std::string Decrypt(std::string& input) {

    if (input.c_str() == NULL)
        return 0;

    size_t length = (input.length() / 8);
    std::string binary;
    binary[0] = '\0';

    for (size_t i = 0; i < length; ++i) {

        srand(i);

        char ch = char(std::bitset<8>(input.substr(i * 8, (i + 1) * 8)).to_ulong());
        for (int j = 7; j >= 0; --j) {
            if (ch & (1 << j)) {
                binary += std::bitset<1>(rand()).to_string();
            }
            else {
                binary += std::bitset<1>(rand()).flip().to_string();
            }
        }
    }

    std::string output;
    std::istringstream in(binary);
    std::bitset<8> bitchar;

    while (in >> bitchar)
    {
        output += char(bitchar.flip().to_ulong());
    }

    return output;
}
