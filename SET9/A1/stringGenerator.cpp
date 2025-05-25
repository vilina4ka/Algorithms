#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <fstream>
 
class StringGenerator {
private:
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";
    const int MIN_LENGTH = 10;
    const int MAX_LENGTH = 200;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist_len; 
    std::uniform_int_distribution<> dist_char;
 
public:
    StringGenerator() : gen(rd()), dist_len(MIN_LENGTH, MAX_LENGTH), dist_char(0, charset.size() - 1) {}

    std::string generateString() {
        int length = dist_len(gen); 
        std::string result;
        result.reserve(length);
        for (int i = 0; i < length; ++i) {
            result += charset[dist_char(gen)]; 
        }
        return result;
    }
 
    std::vector<std::string> generateArray(int size) {
        std::vector<std::string> array;
        array.reserve(size);
        for (int i = 0; i < size; ++i) {
            array.push_back(generateString());
        }
        return array;
    }

    std::vector<std::string> generateReversedSortedArray(int size) {
        auto array = generateArray(size);
        std::sort(array.rbegin(), array.rend());
        return array;
    }

    std::vector<std::string> generateNearlySortedArray(int size) {
        int swaps = size / 10; 
        auto array = generateArray(size);
        std::sort(array.begin(), array.end());
        for (int i = 0; i < swaps; ++i) {
            int idx1 = dist_len(gen) % size;
            int idx2 = dist_len(gen) % size;
            std::swap(array[idx1], array[idx2]);
        }
        return array;
    }
};
 
void writeStringsToFile(const std::string& filename, const std::vector<std::string>& strings) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
 
    for (const auto& str : strings) {
        file << str << std::endl;
    }
 
    file.close();
}
 
int main() {
    StringGenerator generator;
    auto unsorted = generator.generateArray(3000);
    auto reversed = generator.generateReversedSortedArray(3000);
    auto almostSorted = generator.generateNearlySortedArray(3000);
 
    writeStringsToFile("unsorted", unsorted);
    writeStringsToFile("reversed", reversed);
    writeStringsToFile("almost-sorted", almostSorted);
 
    return 0;
}