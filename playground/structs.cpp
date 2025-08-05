#include <iostream>
#include <ostream>

struct Point {
    int x;
    int y;
    void print() const {
        std::cout << x << ", " << y << std::endl;
    }
};

int main() {
    constexpr Point points[] = {{1,2}, {3,1}, {2,3}};
    for (auto p : points) {
        p.print();
    }
    for (auto [x,y]: points) {
        std::cout << x*y << std::endl;
    }
}