#include <cassert>
#include <iostream>
#include <string>

#include <nway.h>

using namespace std::string_literals;

int main(int argc, char** argv) {

    using file = std::vector<std::string>;

    file original = {
        "a",
        "b",
        "c",
        "d",
        "e",
    };

    // deletes third line
    file a = {
        "a",
        "b",
        "d",
        "e",
    };

    // changes fifth line
    file b = {
        "a",
        "b",
        "c",
        "d",
        "x",
    };

    // changes first line
    file c = {
        "x",
        "b",
        "c",
        "d",
        "e",
    };

    auto join = [](std::vector<std::string> strs) {
        std::string res;
        for (auto str : strs) {
            res += str + "\n";
        }
        return res;
    };
    auto diff = nway::diff(original, {a, b, c});
    assert(!nway::has_conflict(diff));
    // prints
    // x
    // b
    // d
    // x
    for (auto line : nway::merge(diff)) {
        std::cout << line << std::endl;
    }
    return 0;
}
