#include <iostream>
#include <string>
#include "../nway.h"

using namespace std::string_literals; // enables s-suffix for std::string literals

int main(int argc, char** argv) {
    auto diff = nway::diff("hello world"s, {"hxxllo world"s, "hello wyyrld"s});
    for (auto [o, others] : diff) {
        std::cout << "original: " << o << std::endl;
        for (size_t i = 0; i < others.size(); i++) {
            std::cout << "alternative " << i << ": " << others[i] << std::endl;
        }
        std::cout << "----------" << std::endl;
    }
    return 0;
}
