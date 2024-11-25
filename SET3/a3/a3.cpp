#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <functional>

class ArrayGenerator {
private:
    std::mt19937 randEngine;

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

    std::vector<int> generateReversedArray(int size) {
        std::vector<int> array = generateRandomArray(size);
        std::sort(array.begin(), array.end());
        std::reverse(array.begin(), array.end());
        return array;
    }

    std::vector<int> generateAlmostSortedArray(int size) {
        std::vector<int> array = generateRandomArray(size);
        std::sort(array.begin(), array.end());
        for (int i = 0; i < size; i += 20) {
            std::swap(array[i], array[i + 1]);
        }
        return array;
    }
};

template <typename Func>
double measureTime(Func sortingFunc, std::vector<int>& array) {
    auto start = std::chrono::high_resolution_clock::now();
    sortingFunc(array);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

class SortTester {
public:
    void runTest(const std::vector<int>& sourceArray, int size, std::function<void(std::vector<int>&)> sortingFunc, int iterations, const std::string& label) {
        double totalTime = 0.0;
        for (int i = 0; i < iterations; ++i) {
            std::vector<int> array = sourceArray;
            array.resize(size);
            totalTime += measureTime(sortingFunc, array);
        }
        std::cout << label << " " << size << " " << (totalTime / iterations) << std::endl;
    }

    void testSort(const std::string& sortName, const std::function<void(std::vector<int>&)>& sortingFunc, ArrayGenerator& generator, int minSize, int maxSize, int step, int iterations) {
        std::cout << "Testing " << sortName << "..." << std::endl;

        for (int size = minSize; size <= maxSize; size += step) {
            auto randomArray = generator.generateRandomArray(size);
            runTest(randomArray, size, sortingFunc, iterations, sortName + " Random");

            auto reversedArray = generator.generateReversedArray(size);
            runTest(reversedArray, size, sortingFunc, iterations, sortName + " Reversed");

            auto almostSortedArray = generator.generateAlmostSortedArray(size);
            runTest(almostSortedArray, size, sortingFunc, iterations, sortName + " AlmostSorted");
        }
    }
};

void quickSort(std::vector<int>& array, int low, int high) {
    if (low < high) {
        int pivot = array[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (array[j] <= pivot) {
                ++i;
                std::swap(array[i], array[j]);
            }
        }
        std::swap(array[i + 1], array[high]);
        int pivotIndex = i + 1;

        quickSort(array, low, pivotIndex - 1);
        quickSort(array, pivotIndex + 1, high);
    }
}

void quickSortWrapper(std::vector<int>& array) {
    quickSort(array, 0, array.size() - 1);
}

void heapify(std::vector<int>& array, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && array[left] > array[largest]) {
        largest = left;
    }
    if (right < n && array[right] > array[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(array[i], array[largest]);
        heapify(array, n, largest);
    }
}

void heapSort(std::vector<int>& array) {
    int n = array.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(array, n, i);
    }
    for (int i = n - 1; i >= 0; --i) {
        std::swap(array[0], array[i]);
        heapify(array, i, 0);
    }
}

void insertionSort(std::vector<int>& array) {
    int n = array.size();
    for (int i = 1; i < n; ++i) {
        int key = array[i];
        int j = i - 1;
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = key;
    }
}

void introsort(std::vector<int>& array, int depthLimit) {
    int n = array.size();
    if (n <= 16) {
        insertionSort(array);
        return;
    }

    if (depthLimit == 0) {
        heapSort(array);
        return;
    }

    int pivot = array[n / 2];
    std::vector<int> left, right;
    for (int i = 0; i < n; ++i) {
        if (array[i] < pivot) {
            left.push_back(array[i]);
        } else if (array[i] > pivot) {
            right.push_back(array[i]);
        }
    }

    introsort(left, depthLimit - 1);
    introsort(right, depthLimit - 1);

    array.clear();
    array.insert(array.end(), left.begin(), left.end());
    array.push_back(pivot);
    array.insert(array.end(), right.begin(), right.end());
}

void introsortWrapper(std::vector<int>& array) {
    int depthLimit = 2 * log(array.size()) / log(2);
    introsort(array, depthLimit);
}

int main() {
    ArrayGenerator generator;
    SortTester tester;

    int minSize = 500, maxSize = 10000, step = 100, iterations = 5;

    tester.testSort("QuickSort", quickSortWrapper, generator, minSize, maxSize, step, iterations);
    tester.testSort("Introsort", introsortWrapper, generator, minSize, maxSize, step, iterations);
    return 0;
}
