#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <string>
#include <tuple>

template <typename T>
std::unordered_map<size_t, size_t>
longest_common_subsequence(const T& a, const T& b) {
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
    std::unordered_map<size_t, size_t> matchings;
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

/**
 * diff3 algorithm generalized to any number of candidates
 * http://www.cis.upenn.edu/~bcpierce/papers/diff3-short.pdf
 */
template <typename T>
std::vector<std::tuple<T, std::vector<T>>>
diff(const T& original, const std::vector<T>& strings) {
    using candidate = std::tuple<size_t, T, std::unordered_map<size_t, size_t>>;
    std::vector<std::tuple<T,std::vector<T>>> result;
    std::vector<candidate> candidates;
    for (auto& str : strings) {
        candidates.emplace_back(0, str, longest_common_subsequence(original, str));
    }
    size_t original_pos = 0;
    while (true) {
        /* i is the number of positions for which all strings are aligned */
        int i = 0;
        auto is_aligned = [&original_pos, &i](const candidate& cand) {
            const auto& [pos, str, map] = cand;
            auto it = map.find(original_pos + i);
            return it != map.end() && it->second == pos + i;
        };
        while (original_pos + i < original.size() && std::all_of(candidates.begin(), candidates.end(), is_aligned)) {
            i++;
        }
        if (i == 0) {
            /* unstable chunk */
            size_t curr_pos = original_pos + 1;
            auto differ = [&curr_pos](const candidate& cand) {
                const auto& [pos, str, map] = cand;
                return map.find(curr_pos) == map.end();
            };
            while (std::any_of(candidates.begin(), candidates.end(), differ)) {
                curr_pos++;
                if (curr_pos >= original.size()) break;
            }
            if (std::any_of(candidates.begin(), candidates.end(), differ)) break;
            std::vector<T> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(T(str.begin() + pos, str.begin () + map[curr_pos]));
                pos = map[curr_pos];
            }
            result.emplace_back(T(original.begin() + original_pos, original.begin() + curr_pos), sequences);
            original_pos = curr_pos;
        } else {
            /* stable chunk */
            std::vector<T> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(T(str.begin() + pos, str.begin() + pos + i));
                pos += i;
            }
            result.emplace_back(T(original.begin() + original_pos, original.begin() + original_pos + i), sequences);
            original_pos += i;
        }
    }
    auto unconsumed = [](const candidate& cand){
        const auto& [pos, str, map] = cand;
        return pos < str.size();
    };
    if (original_pos < original.size() || std::any_of(candidates.begin(), candidates.end(), unconsumed)) {
        std::vector<T> sequences;
        for (auto& [pos, str, map] : candidates) {
            sequences.emplace_back(T(str.begin() + pos, str.end()));
        }
        result.emplace_back(T(original.begin() + original_pos, original.end()), sequences);
    }
    return result;
}

int main() {

    std::string o_s = "helloworld";
    std::string a_s = "helloworlda";
    std::string b_s = "hxxlloworldb";

    std::vector<char> o(o_s.begin(), o_s.end());
    std::vector<char> a(a_s.begin(), a_s.end());
    std::vector<char> b(b_s.begin(), b_s.end());
    auto res = diff(o, {a, b});
    for (auto [ov, x] : res) {
        auto oa = x[0];
        auto ob = x[1];
        if (oa == ob) {
            std::cout << std::string(oa.begin(), oa.end()) << std::endl;
        } else if (ov == oa) {
            std::cout << std::string(ob.begin(), ob.end()) << std::endl;
        } else if (ov == ob) {
            std::cout << std::string(oa.begin(), oa.end()) << std::endl;
        } else {
            std::cout << "<<<<<<<" << std::endl;
            std::cout << std::string(oa.begin(), oa.end()) << std::endl;
            std::cout << "=======" << std::endl;
            std::cout << std::string(ob.begin(), ob.end()) << std::endl;
            std::cout << ">>>>>>>" << std::endl;
        }
    }
    return 0;
}
