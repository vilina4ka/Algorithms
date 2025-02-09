#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <fstream>

class HashTable {
private:
    struct Value {
        int key;
        bool hasValue;
        Value(int k) : key(k), hasValue(true) {}
        Value() : key(0), hasValue(false) {}
    };

    size_t M;
    std::vector<Value> table;
    int collisions = 0;  // Счетчик коллизий

    [[nodiscard]] size_t get_hash(int key) const {
        return key % M;
    }

public:
    explicit HashTable(size_t size) : M(size), table(size) {}

    // Квадратичное пробирование: h(k, i) = h(k) + i + i^2
    bool quadratic_insert(int key) {
        size_t hash = get_hash(key);
        for (size_t i = 0; i < M; ++i) {
            size_t index = (hash + i + i * i) % M;
            if (table[index].hasValue && table[index].key == key) {
                return true;
            }
            if (!table[index].hasValue) {
                table[index] = Value(key);
                return true;
            }
            collisions++;  // Коллизия произошла
        }
        return false;
    }

    // Кубическое пробирование: h(k, i) = h(k) + i + i^2 + i^3
    bool cubic_insert(int key) {
        size_t hash = get_hash(key);
        for (size_t i = 0; i < M; ++i) {
            size_t index = (hash + i + i * i + i * i * i) % M;
            if (table[index].hasValue && table[index].key == key) {
                return true;
            }
            if (!table[index].hasValue) {
                table[index] = Value(key);
                return true;
            }
            collisions++;  // Коллизия произошла
        }
        return false;
    }

    // Подсчет кластеров (групп непрерывных занятых ячеек)
    int count_clusters() const {
        int clusters = 0;
        bool in_cluster = false;
        for (const auto& cell : table) {
            if (cell.hasValue) {
                if (!in_cluster) {
                    clusters++;
                    in_cluster = true;
                }
            } else {
                in_cluster = false;
            }
        }
        return clusters;
    }

    int get_collisions() const {
        return collisions;
    }
};

// Функция тестирования на заданном уровне заполнения
void test_hashing(size_t size, double fill_factor, int iterations, std::ofstream& output) {
    std::cout << "Table size: " << size << ", Fill factor: " << fill_factor * 100 << "%" << std::endl;

    std::random_device rand_dev;
    std::mt19937 rand_engine(rand_dev());
    std::uniform_int_distribution<int> unif(0, 10000);

    std::vector<double> cubic_times, quadratic_times;
    std::vector<int> cubic_clusters, quadratic_clusters;
    std::vector<int> cubic_collisions, quadratic_collisions;

    for (int it = 0; it < iterations; ++it) {
        HashTable quadraticTable(size);
        HashTable cubicTable(size);

        int max_insertions = static_cast<int>(size * fill_factor);
        int inserted = 0;
        int key;

        // Кубическое пробирование
        auto start = std::chrono::high_resolution_clock::now();
        while (inserted < max_insertions) {
            key = unif(rand_engine);
            if (cubicTable.cubic_insert(key)) {
                inserted++;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_cubic = end - start;
        cubic_times.push_back(time_cubic.count());
        cubic_clusters.push_back(cubicTable.count_clusters());
        cubic_collisions.push_back(cubicTable.get_collisions());

        // Квадратичное пробирование
        inserted = 0;
        start = std::chrono::high_resolution_clock::now();
        while (inserted < max_insertions) {
            key = unif(rand_engine);
            if (quadraticTable.quadratic_insert(key)) {
                inserted++;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_quadratic = end - start;
        quadratic_times.push_back(time_quadratic.count());
        quadratic_clusters.push_back(quadraticTable.count_clusters());
        quadratic_collisions.push_back(quadraticTable.get_collisions());
    }

    // Усреднение значений
    double avg_cubic_time = std::accumulate(cubic_times.begin(), cubic_times.end(), 0.0) / iterations;
    double avg_quadratic_time = std::accumulate(quadratic_times.begin(), quadratic_times.end(), 0.0) / iterations;
    double avg_cubic_clusters = std::accumulate(cubic_clusters.begin(), cubic_clusters.end(), 0.0) / iterations;
    double avg_quadratic_clusters = std::accumulate(quadratic_clusters.begin(), quadratic_clusters.end(), 0.0) / iterations;
    double avg_cubic_collisions = std::accumulate(cubic_collisions.begin(), cubic_collisions.end(), 0.0) / iterations;
    double avg_quadratic_collisions = std::accumulate(quadratic_collisions.begin(), quadratic_collisions.end(), 0.0) / iterations;

    // Вывод результатов в консоль
    std::cout << "Cubic Probing (avg over " << iterations << " runs):" << std::endl;
    std::cout << "  Avg Time: " << avg_cubic_time << " sec" << std::endl;
    std::cout << "  Avg Clusters: " << avg_cubic_clusters << std::endl;
    std::cout << "  Avg Collisions: " << avg_cubic_collisions << std::endl;

    std::cout << "Quadratic Probing (avg over " << iterations << " runs):" << std::endl;
    std::cout << "  Avg Time: " << avg_quadratic_time << " sec" << std::endl;
    std::cout << "  Avg Clusters: " << avg_quadratic_clusters << std::endl;
    std::cout << "  Avg Collisions: " << avg_quadratic_collisions << std::endl;

    std::cout << "---------------------------------------\n";

    // Запись данных в файл
    output << size << "," << fill_factor << "," << avg_cubic_time << "," << avg_quadratic_time << ","
           << avg_cubic_clusters << "," << avg_quadratic_clusters << ","
           << avg_cubic_collisions << "," << avg_quadratic_collisions << "\n";
}

// Главная функция
int main() {
    std::ofstream output("results.csv");
    output << "Size,FillFactor,CubicTime,QuadraticTime,CubicClusters,QuadraticClusters,CubicCollisions,QuadraticCollisions\n";

    size_t test_sizes[] = {23, 101, 503, 1009};  // Размеры таблицы
    double fill_factors[] = {0.3, 0.4, 0.5, 0.6, 0.7}; // Уровни заполнения
    int iterations = 30;  // Количество итераций

    for (size_t size : test_sizes) {
        for (double fill_factor : fill_factors) {
            test_hashing(size, fill_factor, iterations, output);
        }
    }

    output.close();
    return 0;
}