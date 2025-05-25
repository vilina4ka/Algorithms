#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <array>

const std::string UNSORTED = "unsorted";
const std::string REVERSED = "reversed";
const std::string ALMOST_SORTED = "almost-sorted";

class TestDataHelper {
public:
    std::vector<std::string> getPartOfArray(const std::vector<std::string>& array, int n) {
        std::vector<std::string> newArray(n);
        for (int i = 0; i < n; ++i) {
            newArray[i] = array[i];
        }
        return newArray;
    }

    std::vector<std::string> readStringsFromFile(const std::string& filename) {
        std::vector<std::string> strings;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return strings;
        }

        std::string line;
        while (getline(file, line)) {
            strings.push_back(line);
        }

        file.close();
        return strings;
    }
};

class MergeSort {
public:
    static int compareCount;

    static void sort(std::vector<std::string>& array) {
        compareCount = 0;
        if (array.size() < 2) return;
        mergeSort(array, 0, array.size() - 1);
    }

private:
    static void mergeSort(std::vector<std::string>& array, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }

    static void merge(std::vector<std::string>& array, int left, int mid, int right) {
        std::vector<std::string> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            compareCount += std::min(array[i].size(), array[j].size());
            if (array[i] <= array[j]) {
                temp[k++] = array[i++];
            } else {
                temp[k++] = array[j++];
            }
        }

        while (i <= mid) {
            temp[k++] = array[i++];
        }
        while (j <= right) {
            temp[k++] = array[j++];
        }

        for (i = left, k = 0; i <= right; i++, k++) {
            array[i] = temp[k];
        }
    }
};

class QuickSort {
public:
    static int compareCount;

    static void sort(std::vector<std::string>& array) {
        compareCount = 0;
        quickSort(array, 0, array.size() - 1);
    }

private:
    static void quickSort(std::vector<std::string>& array, int low, int high) {
        if (low < high) {
            int pi = partition(array, low, high);
            quickSort(array, low, pi - 1);
            quickSort(array, pi + 1, high);
        }
    }

    static int partition(std::vector<std::string>& array, int low, int high) {
        std::string pivot = array[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            compareCount += std::min(array[j].size(), pivot.size());
            if (array[j] < pivot) {
                i++;
                std::swap(array[i], array[j]);
            }
        }
        std::swap(array[i + 1], array[high]);
        return i + 1;
    }
};

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

    static std::vector<std::pair<std::string, int>> stringMerge(const std::vector<std::pair<std::string, int>>& P, const std::vector<std::pair<std::string, int>>& Q) {
        std::vector<std::pair<std::string, int>> R;
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

    static std::vector<std::pair<std::string, int>> stringMergeSort(const std::vector<std::string>& strings) {
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

int MergeSort::compareCount = 0;
int QuickSort::compareCount = 0;
int StringMergeSort::compareCount = 0;
int StringQuickSort::compareCount = 0;
int MSDRadixSort::compareCount = 0;
int HybridMSDQuickSort::compareCount = 0;

class StringSortTester {
public:
    StringSortTester(const std::vector<std::string>& sourceArray, const std::string& dataType)
        : sourceArray_(sourceArray), dataType_(dataType) {}

    template<typename SortClass>
    void testAlgorithm(const std::string& algorithmName) {
        TestDataHelper helper;
        std::cout << algorithmName << " | " << dataType_ << std::endl;
        for (int n = 100; n <= 3000; n += 100) {
            std::vector<std::string> array = helper.getPartOfArray(sourceArray_, n);
            double totalMilliseconds = 0;
            int totalCompareCount = 0;
            for (int i = 0; i < 5; ++i) {
                std::vector<std::string> copiedArray(n);
                std::copy(array.begin(), array.end(), copiedArray.begin());
                auto start = std::chrono::high_resolution_clock::now();
                SortClass::sort(copiedArray);
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                totalMilliseconds += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                totalCompareCount += SortClass::compareCount;
            }
            double avgTime = totalMilliseconds / 5.0;
            int avgCompare = totalCompareCount / 5;
            std::cout << avgTime << " " << avgCompare << std::endl;
        }
        std::cout << std::endl;
    }

private:
    std::vector<std::string> sourceArray_;
    std::string dataType_;
};

int main() {
    TestDataHelper testDataHelper;

    std::vector<std::string> unsortedSourceArray = testDataHelper.readStringsFromFile(UNSORTED);
    std::vector<std::string> reversedSourceArray = testDataHelper.readStringsFromFile(REVERSED);
    std::vector<std::string> almostSortedSourceArray = testDataHelper.readStringsFromFile(ALMOST_SORTED);

    StringSortTester unsortedTester(unsortedSourceArray, "Unsorted");
    StringSortTester reversedTester(reversedSourceArray, "Reversed");
    StringSortTester almostSortedTester(almostSortedSourceArray, "Almost Sorted");

    unsortedTester.testAlgorithm<MergeSort>("Merge");
    reversedTester.testAlgorithm<MergeSort>("Merge");
    almostSortedTester.testAlgorithm<MergeSort>("Merge");

    unsortedTester.testAlgorithm<QuickSort>("Quick");
    reversedTester.testAlgorithm<QuickSort>("Quick");
    almostSortedTester.testAlgorithm<QuickSort>("Quick");

    unsortedTester.testAlgorithm<StringMergeSort>("String Merge");
    reversedTester.testAlgorithm<StringMergeSort>("String Merge");
    almostSortedTester.testAlgorithm<StringMergeSort>("String Merge");

    unsortedTester.testAlgorithm<StringQuickSort>("String Quick");
    reversedTester.testAlgorithm<StringQuickSort>("String Quick");
    almostSortedTester.testAlgorithm<StringQuickSort>("String Quick");

    unsortedTester.testAlgorithm<MSDRadixSort>("Radix");
    reversedTester.testAlgorithm<MSDRadixSort>("Radix");
    almostSortedTester.testAlgorithm<MSDRadixSort>("Radix");

    unsortedTester.testAlgorithm<HybridMSDQuickSort>("Hybrid");
    reversedTester.testAlgorithm<HybridMSDQuickSort>("Hybrid");
    almostSortedTester.testAlgorithm<HybridMSDQuickSort>("Hybrid");

    return 0;
}