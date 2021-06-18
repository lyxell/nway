#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

std::map<size_t,size_t>
longest_common_subsequence(const std::string& a, const std::string& b) {
    std::vector<std::vector<size_t>> arr(a.size() + 1, std::vector<size_t>(b.size() + 1));
    for (size_t i = 0; i <= a.size(); i++) { 
        for (size_t j = 0; j <= b.size(); j++) { 
            if (i == 0 || j == 0) {
                arr[i][j] = 0; 
            } else if (a[i - 1] == b[j - 1]) {
                arr[i][j] = arr[i - 1][j - 1] + 1; 
            } else {
                arr[i][j] = std::max(arr[i - 1][j], arr[i][j - 1]); 
            }
        } 
    }
    std::map<size_t,size_t> matchings;
    size_t i = a.size();
    size_t j = b.size();
    while (i > 0 && j > 0) {
        if (a[i-1] == b[j-1]) { 
            matchings.emplace(i-1, j-1);
            i--;
            j--;
        } else if (arr[i-1][j] > arr[i][j-1]) {
            i--;
        } else {
            j--;
        }
    }
    return matchings;
}

int diff3(const std::string& o, const std::string& a, const std::string& b) {
    auto MA = longest_common_subsequence(o, a);
    auto MB = longest_common_subsequence(o, b);
    size_t lO = 0;
    size_t lA = 0;
    size_t lB = 0;
    while (true) {
        int i = 0;
        while (lO + i < o.size() && MA.find(lO + i) != MA.end() && MA[lO + i] == lA + i && MB.find(lO + i) != MB.end() && MB[lO + i] == lB + i) {
            i++;
        }
        if (i == 0) {
            size_t x = lO + 1;
            while (MA.find(x) == MA.end() || MB.find(x) == MB.end()) {
                x++;
                if (x >= o.size()) break;
            }
            if (MA.find(x) == MA.end() || MB.find(x) == MB.end()) break;
            std::cout << o.substr(lO, x     - lO) << " ";
            std::cout << a.substr(lA, MA[x] - lA) << " ";
            std::cout << b.substr(lB, MB[x] - lB) << std::endl;
            lO = x;
            lA = MA[x];
            lB = MB[x];
        } else {
            std::cout << o.substr(lO, i) << " ";
            std::cout << a.substr(lA, i) << " ";
            std::cout << b.substr(lB, i) << std::endl;
            lO = lO + i;
            lA = lA + i;
            lB = lB + i;
        }
    }
    if (lO < o.size() || lA < a.size() || lB < b.size()) {
        std::cout << o.substr(lO) << " ";
        std::cout << a.substr(lA) << " ";
        std::cout << b.substr(lB) << std::endl;
    }
    return 0;
}

int main() {
    std::string o = "helloworld";
    std::string a = "hellowld";
    std::string b = "hxxlloworld";
    diff3(o, a, b);
    return 0;
}
