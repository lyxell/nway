# nway 

Algorithms to perform n-way diff and merge of files with a common
ancestor, based on the 3-way diff approach described in [A Formal
Investigation of
Diff3](http://www.cis.upenn.edu/~bcpierce/papers/diff3-short.pdf)
by Khanna, Kunal and Pierce and the shortest edit script
algorithm described in [An O(ND) difference algorithm and its
variations](http://www.xmailserver.org/diff2.pdf) by Myers.

Distributed as a single-header C++17 library.

<img src="https://raw.githubusercontent.com/lyxell/nway/master/.github/image.svg" alt="n-way merge with common ancestor">

## Examples

### Merge strings

```c++
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
```

### Merge files

```c++
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

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
```
