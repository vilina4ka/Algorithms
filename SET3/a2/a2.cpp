#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <functional>

class ArrayGenerator {
private:
    std::mt19937 randEngine; 
    const int maxSize = 10000; 
    const int minValue = 0;    
    const int maxValue = 6000; 

    std::mt19937& getRandEngine() {
        return randEngine;
    }

public:
    ArrayGenerator() {
        std::random_device randDevice;
        randEngine = std::mt19937(randDevice());
    }

    std::vector<int> generateRandomArray(int size, int minVal = 0, int maxVal = 6000) {
        std::uniform_int_distribution<int> unif(minVal, maxVal);
        std::vector<int> array(size);
        for (int i = 0; i < size; ++i) {
            array[i] = unif(randEngine);
        }
        return array;
    }

    std::vector<int> getPartOfArray(const std::vector<int>& array, int n) {
        std::vector<int> newArray(n);
        for (int i = 0; i < n; ++i) {
            newArray[i] = array[i];
        }
        return newArray;
    }

    std::vector<int> generateRandomArray() {
        std::uniform_int_distribution<int> unif(0, 3000);
        std::vector<int> array(6000);
        for (int i = 0; i < 6000; ++i) {
            array[i] = unif(randEngine);
        }
        return array;
    }

    std::vector<int> generateReversedArray() {
        std::vector<int> array = generateRandomArray();
        std::sort(array.begin(), array.end());
        std::reverse(array.begin(), array.end());
        return array;
    }

    std::vector<int> generateAlmostSortedArray() {
        std::vector<int> array = generateRandomArray();
        std::sort(array.begin(), array.end());
        for (int i = 0; i < 6000; i += 20) {
            std::swap(array[i], array[i + 1]);
        }
        return array;
    }
};

template <typename Func>
double measureTime(Func sortingFunc, std::vector<int>& array, int threshold) {
    auto start = std::chrono::high_resolution_clock::now();
    sortingFunc(array, threshold);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

class SortTester {
public:
    void runTest(const std::vector<int>& sourceArray, int size, std::function<void(std::vector<int>&, int)> sortingFunc, int iterations, int threshold, const std::string& label) {
        double totalTime = 0.0;
        for (int i = 0; i < iterations; ++i) {
            std::vector<int> array = sourceArray;
            array = ArrayGenerator().getPartOfArray(array, size);
            totalTime += measureTime(sortingFunc, array, threshold);
        }
        std::cout << label << " " << size << " " << threshold << " " << (totalTime / iterations) << std::endl;
    }

    void testSort(const std::string& sortName, const std::function<void(std::vector<int>&, int)>& sortingFunc, ArrayGenerator& generator, int minSize, int maxSize, int step, int iterations, int threshold) {
        std::cout << "Testing " << sortName << "..." << std::endl;

        for (int size = minSize; size <= maxSize; size += step) {
            auto randomArray = generator.generateRandomArray(size);
            runTest(randomArray, size, sortingFunc, iterations, threshold, sortName + " Random");

            auto reversedArray = generator.generateReversedArray();
            runTest(reversedArray, size, sortingFunc, iterations, threshold, sortName + " Reversed");

            auto almostSortedArray = generator.generateAlmostSortedArray();
            runTest(almostSortedArray, size, sortingFunc, iterations, threshold, sortName + " AlmostSorted");
        }
    }
};

void merge(std::vector<int>& array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = array[left + i];
    }
    for (int i = 0; i < n2; ++i) {
        R[i] = array[mid + 1 + i];
    }
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k++] = L[i++];
        }
        else {
            array[k++] = R[j++];
        }
    }
    while (i < n1) {
        array[k++] = L[i++];
    }
    while (j < n2) {
        array[k++] = R[j++];
    }
}

void mergeSort(std::vector<int>& array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(array, left, mid);
        mergeSort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void mergeSortWrapper(std::vector<int>& array, int threshold = 0) {
    mergeSort(array, 0, array.size() - 1);
}

void insertionSort(std::vector<int>& array, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = array[i];
        int j = i - 1;
        while (j >= left && array[j] > key) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = key;
    }
}

void hybridSort(std::vector<int>& array, int left, int right, int threshold) {
    if (right - left + 1 <= threshold) {
        insertionSort(array, left, right);
        return;
    }

    if (left < right) {
        int mid = left + (right - left) / 2;
        hybridSort(array, left, mid, threshold);
        hybridSort(array, mid + 1, right, threshold);
        merge(array, left, mid, right);
    }
}

void hybridSortWrapper(std::vector<int>& array, int threshold = 15) {
    hybridSort(array, 0, array.size() - 1, threshold);
}

int main() {
    ArrayGenerator generator;
    SortTester tester;

    int minSize = 500, maxSize = 10000, step = 100, iterations = 5, threshold = 0;

    tester.testSort("Merge", mergeSortWrapper, generator, minSize, maxSize, step, iterations, threshold);
    
    threshold = 5;
    tester.testSort("Hybrid", hybridSortWrapper, generator, minSize, maxSize, step, iterations, threshold);

    threshold = 10;
    tester.testSort("Hybrid", hybridSortWrapper, generator, minSize, maxSize, step, iterations, threshold);

    threshold = 20;
    tester.testSort("Hybrid", hybridSortWrapper, generator, minSize, maxSize, step, iterations, threshold);

    threshold = 30;
    tester.testSort("Hybrid", hybridSortWrapper, generator, minSize, maxSize, step, iterations, threshold);

    threshold = 50;
    tester.testSort("Hybrid", hybridSortWrapper, generator, minSize, maxSize, step, iterations, threshold);

    return 0;
}

