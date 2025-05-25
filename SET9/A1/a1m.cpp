#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

class StringMergeSort {
public:
    static int compareCount;
 
    static void sort(std::vector<std::string>& array) {
        compareCount = 0;
        auto sortedPairs = stringMergeSort(array);
        for (int i = 0; i < array.size(); ++i) {
            array[i] = sortedPairs[i].first;
        }
    }
 
private:
    static int lcpCompare(const std::string& a, const std::string& b, int index) {
        int n = std::min(a.size(), b.size());
        for (int i = index; i < n; ++i) {
            ++compareCount;
            if (a[i] != b[i]) {
                return i;
            }
        }
        return n;
    }
 
    static std::vector<std::pair<std::string, int> > stringMerge(const std::vector<std::pair<std::string, int> >& P, const std::vector<std::pair<std::string, int> >& Q) {
        std::vector<std::pair<std::string, int> > R;
        int i = 0, j = 0;
        while (i < P.size() && j < Q.size()) {
            if (P[i].first < Q[j].first) {
                compareCount += std::min(P[i].first.size(), Q[j].first.size());
                R.push_back(P[i++]);
            } else if (P[i].first > Q[j].first) {
                compareCount += std::min(P[i].first.size(), Q[j].first.size());
                R.push_back(Q[j++]);
            } else {
                int lcp = lcpCompare(P[i].first, Q[j].first, std::max(P[i].second, Q[j].second));
                if (lcp < P[i].first.size() && lcp < Q[j].first.size()) {
                    if (P[i].first[lcp] < Q[j].first[lcp]) {
                        R.push_back({P[i].first, lcp});
                        i++;
                    } else {
                        R.push_back({Q[j].first, lcp});
                        j++;
                    }
                } else {
                    R.push_back(P[i]);
                    i++;
                    j++;
                }
            }
        }
        while (i < P.size()) R.push_back(P[i++]);
        while (j < Q.size()) R.push_back(Q[j++]);
        return R;
    }
 
    static std::vector<std::pair<std::string, int> > stringMergeSort(const std::vector<std::string>& strings) {
        if (strings.size() <= 1) {
            if (!strings.empty()) {
                return {{strings[0], 0}};
            }
            return {};
        }
        int mid = strings.size() / 2;
        std::vector<std::string> left(strings.begin(), strings.begin() + mid);
        std::vector<std::string> right(strings.begin() + mid, strings.end());
        auto P = stringMergeSort(left);
        auto Q = stringMergeSort(right);
        return stringMerge(P, Q);
    }
};

int StringMergeSort::compareCount = 0;

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    StringMergeSort::sort(arr);
    
    for (const auto& s : arr) {
        std::cout << s << '\n';
    }
}