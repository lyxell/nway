#include <cassert>
#include <iostream>
#include <string>

#include <nway.h>

using namespace std::string_literals;

int main(int argc, char** argv) {
    {
        auto diff = nway::diff("b"s, {"ab"s,
                                      "bc"s});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "abc"s);
    }
    {
        auto diff = nway::diff("hello world"s, {"hxxllo world"s,
                                                "hello wyyrld"s,
                                                "hellu wyyrld!!"s});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "hxxllu wyyrld!!"s);
    }
    {
        auto diff = nway::diff("hello world"s, {});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "hello world"s);
    }
    {
        auto diff = nway::diff(""s, {});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == ""s);
    }
    {
        auto diff = nway::diff("a"s, {"b"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "b"s);
    }
    {
        auto diff = nway::diff("a"s, {"b", "b"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "b"s);
    }
    {
        auto diff = nway::diff(""s, {"b", "b"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "b"s);
    }
    {
        auto diff = nway::diff("ab"s, {"b", "b"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "b"s);
    }
    {
        auto diff = nway::diff("abcdefg"s, {"b", "b"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "b"s);
    }
    {
        auto diff = nway::diff("abcdefg"s, {"", ""});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == ""s);
    }
    {
        auto diff = nway::diff("abcdefg"s, {"hello", "world"});
        assert(nway::has_conflict(diff));
    }
    {
        auto diff = nway::diff(""s, {"hello", "world"});
        assert(nway::has_conflict(diff));
    }
    {
        auto diff = nway::diff(""s, {"", "", ""});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == ""s);
    }
    {
        auto diff = nway::diff(""s, {"a", "a", "a"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "a"s);
    }
    {
        auto diff = nway::diff("zz"s, {"a", "a", "a"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "a"s);
    }
    {
        auto diff = nway::diff("zz"s, {"zza", "zza", "zza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "zza"s);
    }
    {
        auto diff = nway::diff("zz"s, {"zz", "zz", "zza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "zza"s);
    }
    {
        auto diff = nway::diff("zz"s, {"zza", "zz", "zza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "zza"s);
    }
    {
        auto diff = nway::diff("hello world"s, {"zza", "", "zza"});
        assert(nway::has_conflict(diff));
    }
    {
        auto diff = nway::diff("zz"s, {"", "zz", "zz"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == ""s);
    }
    {
        auto diff = nway::diff("zz"s, {"zz", "zz", ""});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == ""s);
    }
    {
        auto diff = nway::diff("zz"s, {"", "zz", ""});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == ""s);
    }
    {
        auto diff = nway::diff("xzz"s, {"x", "xzz", "x"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "x"s);
    }
    {
        auto diff = nway::diff("xzz"s, {"xz", "xzz", "xz"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "xz"s);
    }
    {
        auto diff = nway::diff("xzz"s, {"zxz", "xzz", "zxz"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "zxz"s);
    }
    {
        auto diff = nway::diff("xzzxzzxzz"s, {"axzzxzzxzz", "xzzxzzxzz", "xzzxzzxzza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "axzzxzzxzza"s);
    }
    {
        auto diff = nway::diff("xzzxzzxzz"s, {"axzzxzzxzz", "axzzxzzxzz", "xzzxzzxzza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "axzzxzzxzza"s);
    }
    {
        auto diff = nway::diff("xzzxzzxzz"s, {"axzzxzzxzz", "axzzxzzxzza", "xzzxzzxzza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "axzzxzzxzza"s);
    }
    {
        auto diff = nway::diff("xzzxzzxzz"s, {"axzzxzzxzz", "axzzxzzxzza", "xzzxzzbxzza"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "axzzxzzbxzza"s);
    }
    {
        auto diff = nway::diff("zzz"s, {"zzzz", "zzzz", "zzzz"});
        assert(!nway::has_conflict(diff));
        assert(nway::merge(diff) == "zzzz"s);
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
