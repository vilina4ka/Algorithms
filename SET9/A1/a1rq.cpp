#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

class StringQuickSort {
public:
    static int compareCount;

    static void sort(std::vector<std::string>& strings) {
        compareCount = 0;
        stringQuickSort(strings, 0, strings.size(), 0);
    }

private:
    static void stringQuickSort(std::vector<std::string>& strings, int start, int end, int index) {
        if (end - start <= 1) return;

        if (index >= strings[0].size()) return;

        int pivotIndex = start + (end - start) / 2;
        char pivot = strings[pivotIndex][index];

        int less = start;
        int equal = start;
        int greater = end;

        while (equal < greater) {
            compareCount++;
            if (index < strings[equal].size() && strings[equal][index] < pivot) {
                std::swap(strings[less++], strings[equal++]);
            } else if (index < strings[equal].size() && strings[equal][index] == pivot) {
                ++equal;
            } else {
                std::swap(strings[equal], strings[--greater]);
            }
        }

        stringQuickSort(strings, start, less, index);
        if (index < strings[0].size()) {
            stringQuickSort(strings, less, greater, index + 1);
        }
        stringQuickSort(strings, greater, end, index);
    }
};

class HybridMSDQuickSort {
public:
    static int compareCount;

    static void sort(std::vector<std::string>& array) {
        compareCount = 0;
        hybridMSDQuickSort(array, 0, array.size(), 0, findMaxLength(array));
    }

private:
    static void hybridMSDQuickSort(std::vector<std::string>& array, int start, int end, int index, int maxLength) {
        if (start >= end - 1 || index >= maxLength) return;

        if (end - start < 50) {
            std::vector<std::string> subArray(array.begin() + start, array.begin() + end);
            StringQuickSort::sort(subArray);
            std::copy(subArray.begin(), subArray.end(), array.begin() + start);
            return;
        }

        std::array<std::vector<std::string>, 128> buckets;
        for (int i = start; i < end; i++) {
            compareCount++;
            char c = index < array[i].size() ? array[i][index] : 0;
            buckets[c].push_back(array[i]);
        }

        int pos = start;
        for (auto& bucket : buckets) {
            if (!bucket.empty()) {
                int bucketSize = bucket.size();
                hybridMSDQuickSort(bucket, 0, bucketSize, index + 1, maxLength);
                std::copy(bucket.begin(), bucket.end(), array.begin() + pos);
                pos += bucketSize;
            }
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

int StringQuickSort::compareCount = 0;
int HybridMSDQuickSort::compareCount = 0;

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    HybridMSDQuickSort::sort(arr);
    
    for (const auto& s : arr) {
        std::cout << s << '\n';
    }
}