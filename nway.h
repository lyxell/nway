/**
 * Copyright (c) 2021, Anton Lyxell
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <algorithm>
#include <tuple>
#include <vector>

namespace nway {

/**
 * Compute the longest common subsequence of A and B.
 *
 * Uses O(ND) time and O(D²) space where N = |A| + |B| and D is the size of the
 * minimum edit script for A and B.
 *
 * Returns a map of matchings M such that (i,j) ∈ M ⇒ A[i] = B[j].
 *
 * Based on Myers, Eugene W. "An O(ND) difference algorithm and its
 * variations." Algorithmica 1.1-4 (1986): 251-266.
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
 * Compute a n-way diff of a set of sequences against a common ancestor.
 *
 * Based on the 3-way approach described by Khanna, Sanjeev, Keshav Kunal, and
 * Benjamin C. Pierce. "A formal investigation of diff3." International
 * Conference on Foundations of Software Technology and Theoretical Computer
 * Science. Springer, Berlin, Heidelberg, 2007.
 */
template <typename T>
std::vector<std::tuple<T, std::vector<T>>> diff(const T& ancestor,
                                                const std::vector<T>& seqs) {
    using candidate = std::tuple<size_t, T, std::unordered_map<size_t, size_t>>;
    std::vector<std::tuple<T, std::vector<T>>> result;
    std::vector<candidate> candidates;
    for (auto& seq : seqs) {
        candidates.emplace_back(0, seq,
                                longest_common_subsequence(ancestor, seq));
    }
    size_t ancestor_pos = 0;
    while (true) {
        /* i is the number of positions for which all sequences are aligned */
        int i = 0;
        auto is_aligned = [&ancestor_pos, &i](const candidate& cand) {
            const auto& [pos, str, map] = cand;
            auto it = map.find(ancestor_pos + i);
            return it != map.end() && it->second == pos + i;
        };
        while (ancestor_pos + i < ancestor.size() &&
               std::all_of(candidates.begin(), candidates.end(), is_aligned)) {
            i++;
        }
        if (i == 0) {
            /* unstable chunk */
            size_t curr_pos = ancestor_pos + 1;
            auto differ = [&curr_pos](const candidate& cand) {
                const auto& [pos, str, map] = cand;
                return map.find(curr_pos) == map.end();
            };
            while (std::any_of(candidates.begin(), candidates.end(), differ)) {
                curr_pos++;
                if (curr_pos >= ancestor.size())
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
                T(ancestor.begin() + ancestor_pos, ancestor.begin() + curr_pos),
                sequences);
            ancestor_pos = curr_pos;
        } else {
            /* stable chunk */
            std::vector<T> sequences;
            for (auto& [pos, str, map] : candidates) {
                sequences.emplace_back(
                    T(str.begin() + pos, str.begin() + pos + i));
                pos += i;
            }
            result.emplace_back(T(ancestor.begin() + ancestor_pos,
                                  ancestor.begin() + ancestor_pos + i),
                                sequences);
            ancestor_pos += i;
        }
    }
    auto unconsumed = [](const candidate& cand) {
        const auto& [pos, str, map] = cand;
        return pos < str.size();
    };
    if (ancestor_pos < ancestor.size() ||
        std::any_of(candidates.begin(), candidates.end(), unconsumed)) {
        std::vector<T> sequences;
        for (auto& [pos, str, map] : candidates) {
            sequences.emplace_back(T(str.begin() + pos, str.end()));
        }
        result.emplace_back(T(ancestor.begin() + ancestor_pos, ancestor.end()),
                            sequences);
    }
    return result;
}

/**
 * Check whether a diff has a conflict.
 */
template <typename T>
bool has_conflict(const std::vector<std::tuple<T, std::vector<T>>>& d) {
    for (const auto& [original, candidates] : d) {
        /* collect sequences that differ from original */
        std::vector<size_t> unstable_idx;
        for (size_t i = 0; i < candidates.size(); i++) {
            if (candidates[i] != original) {
                unstable_idx.emplace_back(i);
            }
        }
        /* compare sequences that differ from original */
        for (size_t i = 1; i < unstable_idx.size(); i++) {
            if (candidates[unstable_idx[0]] != candidates[unstable_idx[i]]) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Merge a diff into a single sequence. 
 */
template <typename T>
T merge(const std::vector<std::tuple<T, std::vector<T>>>& d) {
    T result;
    for (const auto& [original, candidates] : d) {
        auto element = original;
        for (auto candidate : candidates) {
            if (candidate != original) {
                element = candidate;
                break;
            }
        }
        result.insert(result.end(), element.begin(), element.end());
    }
    return result;
}

} // namespace nway
