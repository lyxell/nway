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

std::vector<std::tuple<std::string,std::string,std::string>> diff3(const std::string& O, const std::string& A, const std::string& B) {
    std::vector<std::tuple<std::string,std::string,std::string>> result;
    auto MA = longest_common_subsequence(O, A);
    auto MB = longest_common_subsequence(O, B);
    size_t lO = 0;
    size_t lA = 0;
    size_t lB = 0;
    while (true) {
        int i = 0;
        while (lO + i < O.size() && MA.find(lO + i) != MA.end() && MA[lO + i] == lA + i && MB.find(lO + i) != MB.end() && MB[lO + i] == lB + i) {
            i++;
        }
        if (i == 0) {
            size_t o = lO + 1;
            while (MA.find(o) == MA.end() || MB.find(o) == MB.end()) {
                o++;
                if (o >= O.size()) break;
            }
            if (MA.find(o) == MA.end() || MB.find(o) == MB.end()) break;
            result.emplace_back(O.substr(lO, o     - lO),
                                A.substr(lA, MA[o] - lA),
                                B.substr(lB, MB[o] - lB));
            lO = o;
            lA = MA[o];
            lB = MB[o];
        } else {
            result.emplace_back(O.substr(lO, i),
                                A.substr(lA, i),
                                B.substr(lB, i));
            lO = lO + i;
            lA = lA + i;
            lB = lB + i;
        }
    }
    if (lO < O.size() || lA < A.size() || lB < B.size()) {
        result.emplace_back(O.substr(lO), A.substr(lA), B.substr(lB));
    }
    return result;
}

int main() {
    std::string o = "helloworld";
    std::string a = "helloworlda";
    std::string b = "hxxlloworldb";
    auto res = diff3(o, a, b);
    for (auto [ov, oa, ob] : res) {
        if (oa == ob) {
            std::cout << oa << std::endl;
        } else if (ov == oa) {
            std::cout << ob << std::endl;
        } else if (ov == ob) {
            std::cout << oa << std::endl;
        } else {
            std::cout << "<<<<<<<" << std::endl;
            std::cout << oa << std::endl;
            std::cout << "=======" << std::endl;
            std::cout << ob << std::endl;
            std::cout << ">>>>>>>" << std::endl;
        }
    }
    return 0;
}
