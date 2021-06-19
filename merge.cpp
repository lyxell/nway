#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <string>
#include <tuple>

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

using candidate = std::tuple<size_t, std::string, std::map<size_t,size_t>>;

std::vector<std::tuple<std::string,std::vector<std::string>>>
diff(const std::string& O, const std::vector<std::string>& strings) {
    std::vector<std::tuple<std::string,std::vector<std::string>>> result;
    std::vector<candidate> candidates;
    for (auto& str : strings) {
        candidates.emplace_back(0, str, longest_common_subsequence(O, str));
    }
    size_t lO = 0;
    while (true) {
        int i = 0;
        auto agree = [&lO, &i](const candidate& cand) {
            const auto& [pos, str, map] = cand;
            auto it = map.find(lO + i);
            return it != map.end() && it->second == pos + i;
        };
        while (lO + i < O.size() && std::all_of(candidates.begin(), candidates.end(), agree)) {
            i++;
        }
        if (i == 0) {
            size_t o = lO + 1;
            auto differ = [&o](const candidate& cand) {
                const auto& [pos, str, map] = cand;
                return map.find(o) == map.end();
            };
            while (std::any_of(candidates.begin(), candidates.end(), differ)) {
                o++;
                if (o >= O.size()) break;
            }
            if (std::any_of(candidates.begin(), candidates.end(), differ)) break;
            std::vector<std::string> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(str.substr(pos, map[o] - pos));
                pos = map[o];
            }
            result.emplace_back(O.substr(lO, o - lO), sequences);
            lO = o;
        } else {
            std::vector<std::string> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(str.substr(pos, i));
                pos += i;
            }
            result.emplace_back(O.substr(lO, i), sequences);
            lO += i;
        }
    }
    auto unconsumed = [](const candidate& cand){
        const auto& [pos, str, map] = cand;
        return pos < str.size();
    };
    if (lO < O.size() || std::any_of(candidates.begin(), candidates.end(), unconsumed)) {
        std::vector<std::string> sequences;
        for (auto& [pos, str, map] : candidates) {
            sequences.emplace_back(str.substr(pos));
        }
        result.emplace_back(O.substr(lO), sequences);
    }
    return result;
}

int main() {
    std::string o = "helloworld";
    std::string a = "helloworlda";
    std::string b = "hxxlloworldb";
    auto res = diff(o, {a, b});
    for (auto [ov, x] : res) {
        auto oa = x[0];
        auto ob = x[1];
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
