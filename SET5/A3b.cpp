#include <iostream>
#include <vector>

int main() {
    int max_p = 31;
    for (int p = 1; p <= max_p; ++p) {
        std::vector<std::pair<char, char> > neutral_pairs;

        for (char a = 'a'; a <= 'z'; ++a) {
            int x = (a - 'a' + 1);
            if (x % p == 0) {  
                char b = 'a' + 1 - (x / p);
                if (b >= 'a' && b <= 'z') { 
                    neutral_pairs.emplace_back(a, b);
                }
            }
        }

        std::cout << "p = " << p << ": ";
        if (neutral_pairs.empty()) {
            std::cout << "Нет пар\n";
        } else {
            for (auto [a, b] : neutral_pairs) {
                std::cout << "(" << a << ", " << b << ") ";
            }
            std::cout << "\n";
        }
    }
    return 0;
}
