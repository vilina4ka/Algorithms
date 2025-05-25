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

int StringQuickSort::compareCount = 0;

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    
    StringQuickSort::sort(arr);
    
    for (const auto& s : arr) {
        std::cout << s << '\n';
    }
}