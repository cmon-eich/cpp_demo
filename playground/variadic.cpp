#include <iostream>
#include <cstdarg>

void print1() {
    std::cout << std::endl;
}

template<typename... args>
void print1(const int first, args... rest) {
    std::cout << first << " ";
    print1(rest...);
}

template<typename... args>
void print2(args... items) {
    ((std::cout << items << " "), ...);
    std::cout << std::endl;
}

double multiplyAndPrint(double a, double b) {
    const double res = a*b;
    std::cout << res << std::endl;
    return res;
}

template<typename... args>
double square(args... items) {
    double res = 1;
    ((res = multiplyAndPrint(res, items)), ...);
    return res;
}

void print3(const int count, ...) {
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        const int val = va_arg(args, int);
        std::cout << val << " ";
    }
    va_end(args);
    std::cout << std::endl;
}

int main() {
    print1(1,2,3,4,5,6);
    print2(9,8,7,6,5,4,3,2,1);
    square(9,8,7,6,5,4,3,2,1);
    print3(10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
}