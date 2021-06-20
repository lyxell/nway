# nway 

Algorithms to perform n-way diff and merge of files with a common
ancestor, based on the 3-way diff approach described in [A Formal
Investigation of
Diff3](http://www.cis.upenn.edu/~bcpierce/papers/diff3-short.pdf)
by Khanna, Kunal and Pierce and the shortest edit script
algorithm described in [An O(ND) difference algorithm and its
variations](http://www.xmailserver.org/diff2.pdf) by Myers.

Distributed as a single-header library.

Uses C++17.

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
                                            "hello wyyrld!!"s});
    assert(!nway::has_conflict(diff));
    // prints hxxllo wyyrld!!
    std::cout << nway::merge(diff) << std::endl;
    return 0;
}
```
