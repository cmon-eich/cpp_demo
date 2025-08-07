#include <iostream>
#include <limits>

enum Unit {
    Meters,
    Millimeters
};

class Measure {
    int dimension;
    long double m;
    long double mm;
public:
    explicit Measure(const long double measure, const Unit unit, const int dimension) : dimension(dimension) {
        switch (unit) {
            case Meters:
                m = measure;
                mm = measure*1000;
                break;
            case Millimeters:
                m = measure/1000;
                mm = measure;
                break;
        }
    }
    explicit Measure(const long double measure, const Unit unit) : Measure(measure, unit, 1) {}
    long double getMeasure(const Unit u) const {
        switch (u) {
            case Meters:
                return m;
            case Millimeters:
                return mm;
        }
        throw std::invalid_argument("Unrecognized unit");
    }
    int getDimension() const {
        return dimension;
    }
    friend std::ostream& operator<<(std::ostream& os, const Measure& m) {
        if (m.getDimension() == 1) {
            os << m.getMeasure(Meters) << "m";
        } else {
            os << m.getMeasure(Meters) << "m^" << m.getDimension();
        }
        return os;
    }
};

Measure operator+(const Measure& x, const Measure& y) {
    if (x.getDimension() != y.getDimension()) {
        throw std::logic_error("Dimension mismatch");
    }
    return Measure(x.getMeasure(Meters)+y.getMeasure(Meters), Meters, x.getDimension());
}

Measure operator-(const Measure& x, const Measure& y) {
    if (x.getDimension() != y.getDimension()) {
        throw std::logic_error("Dimension mismatch");
    }
    return Measure(x.getMeasure(Meters)-y.getMeasure(Meters), Meters, x.getDimension());
}

Measure operator*(const Measure& x, const Measure& y) {
    return Measure(x.getMeasure(Meters)*y.getMeasure(Meters), Meters, x.getDimension()+y.getDimension());
}

Measure operator/(const Measure& x, const Measure& y) {
    const int dim = x.getMeasure(Meters)-y.getMeasure(Meters);
    if (dim <= 0) throw std::logic_error("You can not divide m^" + std::to_string(x.getDimension()) + " by m^" + std::to_string(y.getDimension()) + "!");
    return Measure(x.getMeasure(Meters)/y.getMeasure(Meters), Meters, dim);
}

long double operator%(const Measure& x, const Measure& y) {
    if (x.getMeasure(Meters)-y.getMeasure(Meters) != 0)
        throw std::logic_error("You can not divide m^" + std::to_string(x.getDimension()) + " by m^" + std::to_string(y.getDimension()) + "!");
    return x.getMeasure(Meters)/y.getMeasure(Meters);
}

Measure operator""_m(const long double m) {
    return Measure(m, Meters);
}
Measure operator""_mm(const long double mm) {
    return Measure(mm, Millimeters);
}
Measure operator""_m(const unsigned long long m) {
    return Measure(m, Meters);
}
Measure operator""_mm(const unsigned long long mm) {
    return Measure(mm, Millimeters);
}

void requestInput(long double* ptr) {
    while (!(std::cin >> *ptr)) {
        std::cerr << "Invalid input. Please enter a number!\n";
        std::cin.clear();
        // Discard invalid input from the buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    auto x = 3.14_m;
    auto y = 90_mm;
    auto z = Measure(14.55, Millimeters);
    std::cout << "This programm is about measures." << std::endl
    << "For measures like:" << std::endl << "x = " << x << std::endl << "y = " << y << std::endl << "z = " << z << std::endl
    << "It is supposed to calculate their cube: " << x*y*z << std::endl;

    // interactive part
    long double length;
    long double width;
    long double height;
    std::cout << "Enter length: ";
    try {
        requestInput(&length);
    } catch (std::invalid_argument& e) {
        return 1;
    }
    std::cout << "Enter width: ";
    try {
        requestInput(&width);
    } catch (std::invalid_argument& e) {
        return 1;
    }
    std::cout << "Enter height: ";
    try {
        requestInput(&height);
    } catch (std::invalid_argument& e) {
        return 1;
    }
    x = Measure(length, Meters);
    y = Measure(width, Meters);
    z = Measure(height, Meters);
    std::cout << "The measures entered would result in a cube with the following dimensions: " << x*y*z << std::endl;
    return 0;
}