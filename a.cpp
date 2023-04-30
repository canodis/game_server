#include <iostream>
#include <string>

#include <iostream>
#include <string>
#include <bitset>

void print_string_bits(const std::string& input) {
    for (const char c : input) {
        std::bitset<8> binary_representation(c);
        std::cout << binary_representation << ' ';
    }
    std::cout << std::endl;
}

int main() {
    std::string input;
    std::cout << "Bir metin girin: ";
    std::getline(std::cin, input);

    std::cout << "Metnin bit gösterimi: ";
    print_string_bits(input);

    return 0;
}


