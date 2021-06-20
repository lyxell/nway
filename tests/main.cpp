#include <cassert>
#include <iostream>
#include <string>

#include <nway.h>

using namespace std::string_literals;

int main(int argc, char** argv) {
    {
        auto diff = nway::diff("hello world"s, {"hxxllo world"s,
                                                "hello wyyrld"s,
                                                "hello wyyrld!!"s});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "hxxllo wyyrld!!"s);
    }
    {
        auto diff = nway::diff("hello world"s, {});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "hello world"s);
    }
    for (size_t i = 0; i < 10; i++) {
        std::vector<std::string> candidates;
        for (size_t j = 0; j < i; j++) {
            candidates.emplace_back("hello world"s);
        }
        auto diff = nway::diff("hello world"s, candidates);
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "hello world"s);
    }
    std::cerr << "all tests passed" << std::endl;
    return 0;
}
