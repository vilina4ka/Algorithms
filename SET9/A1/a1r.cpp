#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

class MSDRadixSort {
public:
    static int compareCount;

    static void sort(std::vector<std::string>& array) {
        compareCount = 0;
        int maxLength = findMaxLength(array);
        msdRadixSort(array, 0, array.size(), 0, maxLength);
    }

private:
    static void msdRadixSort(std::vector<std::string>& array, int start, int end, int index, int maxLength) {
        if (start >= end - 1 || index >= maxLength) return;

        std::array<std::vector<std::string>, 128> buckets;

        for (int i = start; i < end; i++) {
            compareCount++;
            char c = index < array[i].size() ? array[i][index] : 0;
            buckets[c].push_back(array[i]);
        }

        int pos = start;
        for (auto& bucket : buckets) {
            for (auto& s : bucket) {
                array[pos++] = s;
            }
            msdRadixSort(array, pos - bucket.size(), pos, index + 1, maxLength);
            bucket.clear();
        }
    }

    static int findMaxLength(const std::vector<std::string>& array) {
        int maxLength = 0;
        for (const auto& str : array) {
            if (str.length() > maxLength) {
                maxLength = str.length();
            }
        }
        return maxLength;
    }
};

int MSDRadixSort::compareCount = 0;

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    MSDRadixSort::sort(arr);
    
    for (const auto& s : arr) {
        std::cout << s << '\n';
    }
}