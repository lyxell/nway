#include <cassert>
#include <iostream>
#include <string>

#include <nway.h>

using namespace std::string_literals;

int main(int argc, char** argv) {
    auto diff = nway::diff("hello world"s, {"hxxllo world"s,
                                            "hello wyyrld"s,
                                            "hello wyyrld!!"s,
                                            "a hello world"s});
    assert(!nway::has_conflict(diff));
    // prints "a hxxllo wyyrld!!"
    std::cout << nway::merge(diff) << std::endl;
    return 0;
}
