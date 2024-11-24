#include <iostream>
#include <random>
#include <cmath>

bool isInIntersection(double x, double y, double r1, double r2, double r3, double x1, double y1, double x2, double y2, double x3, double y3) {

    return (std::pow(x - x1, 2) + std::pow(y - y1, 2) <= std::pow(r1, 2)) &&
           (std::pow(x - x2, 2) + std::pow(y - y2, 2) <= std::pow(r2, 2)) &&
           (std::pow(x - x3, 2) + std::pow(y - y3, 2) <= std::pow(r3, 2));
}

void monteCarlo(int N, double r1, double r2, double r3, 
                           double x1, double y1, double x2, double y2, double x3, double y3) {
    double leftMin = std::max(x1 - r1, std::max(x2 - r2, x3 - r3)); 
    double rightMin = std::min(x1 + r1, std::min(x2 + r2, x3 + r3)); 
    double upMin = std::min(y1 + r1, std::min(y2 + r2, y3 + r3)); 
    double downMin = std::max(y1 - r1, std::max(y2 - r2, y3 - r3)); 
    double sRecMin =  (rightMin - leftMin) * (upMin - downMin);

    double leftMax = std::min(x1 - r1, std::min(x2 - r2, x3 - r3)); 
    double rightMax = std::max(x1 + r1, std::max(x2 + r2, x3 + r3)); 
    double upMax = std::max(y1 + r1, std::max(y2 + r2, y3 + r3)); 
    double downMax = std::min(y1 - r1, std::min(y2 - r2, y3 - r3));
    double sRecMax =  (rightMax - leftMax) * (upMax - downMax); 
    
    double exact = 0.25 * M_PI + 1.25 * std::asin(0.8) - 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    int M = 0;
    std::uniform_real_distribution<> disXmax(leftMax, rightMax);  
    std::uniform_real_distribution<> disYmax(downMax, upMax);
    for (int i = 0; i < N; ++i) {
        double x = disXmax(gen); 
        double y = disYmax(gen);
        if (isInIntersection(x, y, r1, r2, r3, x1, y1, x2, y2, x3, y3)) {
            M++;
        }
    }
    double Se =  (M / static_cast<double>(N)) * sRecMax;
    std::cout <<  "max" << " " << N  << " " << Se << " " << 
    (std::abs(exact - Se) / exact)* 100 << std::endl;    

    M = 0;
    std::uniform_real_distribution<> disXmin(leftMin, rightMin);  
    std::uniform_real_distribution<> disYmin(downMin, upMin);
    for (int i = 0; i < N; ++i) {
        double x = disXmin(gen); 
        double y = disYmin(gen);
        if (isInIntersection(x, y, r1, r2, r3, x1, y1, x2, y2, x3, y3)) {
            M++;
        }
    }
    Se =  (M / static_cast<double>(N)) * sRecMin;
    std::cout <<  "min" << " " << N  << " " << Se << " " << 
    (std::abs(exact - Se) / exact)* 100 << std::endl;

} 

int main() {
    double r1 = 1.0, r2 = std::pow(5, 0.5) / 2, r3 = r2;
    double x1 = 1.0, y1 = 1.0; 
    double x2 = 1.5, y2 = 2.0;  
    double x3 = 2.0, y3 = 1.5; 
    for (int N = 1000; N <= 100000; N+=500) {
        monteCarlo(N, r1, r2, r3, x1, y1, x2, y2, x3, y3);
    }
    return 0;
}
