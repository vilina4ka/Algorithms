#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iostream>

class StringSortTester {
public:
    StringSortTester(const std::vector<std::string>& sourceArray, const std::string& dataType)
        : sourceArray_(sourceArray), dataType_(dataType) {}

    template<typename SortClass>
    void testAlgorithm(const std::string& algorithmName) {
        std::cout << algorithmName << " | " << dataType_ << std::endl;
        for (int n = 100; n <= 3000; n += 100) {
            std::vector<std::string> array(sourceArray_.begin(), sourceArray_.begin() + n);
            double totalMilliseconds = 0;
            for (int i = 0; i < 5; ++i) {
                auto arrCopy = array;
                auto start = std::chrono::high_resolution_clock::now();
                SortClass::sort(arrCopy);
                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now() - start
                );
                totalMilliseconds += elapsed.count();
            }
            std::cout << totalMilliseconds / 5 << " ";
        }
        std::cout << "\n\n";
    }

private:
    std::vector<std::string> sourceArray_;
    std::string dataType_;
};

int main() { 
    return 0; 
}