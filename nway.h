#pragma once
#include <algorithm>
#include <tuple>
#include <vector>

namespace nway {

/**
 * Compute the longest common subsequence of A and B.
 * Uses O(ND) time and O(D²) space where N = |A| + |B| and D is the size of the
 * minimum edit script for A and B.
 *
 * Returns a map of matchings M such that (i,j) ∈ M ⇒ A[i] = B[j].
 *
 * See E. Myers (1986). "An O(ND) Difference Algorithm and Its Variations" for
 * reference.
 */
template <typename T>
std::unordered_map<size_t, size_t> longest_common_subsequence(const T& a,
                                                              const T& b) {
    std::unordered_map<long, long> v;
    std::vector<decltype(v)> vs;
    for (long d = 0; d <= long(a.size() + b.size()); d++) {
        vs.emplace_back(v);
        for (long k = -d; k <= d; k += 2) {
            long x = 0;
            long y = 0;
            if (k == -d || (k != d && v[k - 1] < v[k + 1])) {
                x = v[k + 1];
            } else {
                x = v[k - 1] + 1;
            }
            y = x - k;
            while (x < a.size() && y < b.size() && a[x] == b[y]) {
                x++;
                y++;
            }
            v[k] = x;
            if (x >= a.size() && y >= b.size()) {
                /* backtracking */
                std::unordered_map<size_t, size_t> matchings;
                while (vs.size()) {
                    if (k == -d || (k != d && v[k - 1] < v[k + 1])) {
                        k++;
                    } else {
                        k--;
                    }
                    while (x > v[k] && y > v[k] - k) {
                        x--;
                        y--;
                        matchings.emplace(x, y);
                    }
                    x = v[k];
                    y = v[k] - k;
                    k = x - y;
                    if (v.empty())
                        break;
                    v = vs.back();
                    vs.pop_back();
                    d--;
                }
                return matchings;
            }
        }
    }
    return {};
}

/**
 * diff3 algorithm generalized to any number of candidates
 * http://www.cis.upenn.edu/~bcpierce/papers/diff3-short.pdf
 */
template <typename T>
std::vector<std::tuple<T, std::vector<T>>> diff(const T& original,
                                                const std::vector<T>& seqs) {
    using candidate = std::tuple<size_t, T, std::unordered_map<size_t, size_t>>;
    std::vector<std::tuple<T, std::vector<T>>> result;
    std::vector<candidate> candidates;
    for (auto& seq : seqs) {
        candidates.emplace_back(0, seq,
                                longest_common_subsequence(original, seq));
    }
    size_t original_pos = 0;
    while (true) {
        /* i is the number of positions for which all sequences are aligned */
        int i = 0;
        auto is_aligned = [&original_pos, &i](const candidate& cand) {
            const auto& [pos, str, map] = cand;
            auto it = map.find(original_pos + i);
            return it != map.end() && it->second == pos + i;
        };
        while (original_pos + i < original.size() &&
               std::all_of(candidates.begin(), candidates.end(), is_aligned)) {
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
                if (curr_pos >= original.size())
                    break;
            }
            if (std::any_of(candidates.begin(), candidates.end(), differ))
                break;
            std::vector<T> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(
                    T(str.begin() + pos, str.begin() + map[curr_pos]));
                pos = map[curr_pos];
            }
            result.emplace_back(
                T(original.begin() + original_pos, original.begin() + curr_pos),
                sequences);
            original_pos = curr_pos;
        } else {
            /* stable chunk */
            std::vector<T> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(
                    T(str.begin() + pos, str.begin() + pos + i));
                pos += i;
            }
            result.emplace_back(T(original.begin() + original_pos,
                                  original.begin() + original_pos + i),
                                sequences);
            original_pos += i;
        }
    }
    auto unconsumed = [](const candidate& cand) {
        const auto& [pos, str, map] = cand;
        return pos < str.size();
    };
    if (original_pos < original.size() ||
        std::any_of(candidates.begin(), candidates.end(), unconsumed)) {
        std::vector<T> sequences;
        for (auto& [pos, str, map] : candidates) {
            sequences.emplace_back(T(str.begin() + pos, str.end()));
        }
        result.emplace_back(T(original.begin() + original_pos, original.end()),
                            sequences);
    }
    return result;
}

} // namespace nway
