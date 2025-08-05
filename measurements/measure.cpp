#include <type_traits>
#include <iostream>
#include <map>
#include <ostream>
#include <string>

struct Length;
struct Weight;

enum LengthUnit {
    Kilometer,
    Meter,
    Decimeter,
    Centimeter,
    Millimeter,
    Micrometer,
    Nanometer,
};

enum WeightUnit {
    Kilogramm,
    Gramm,
    Milligramm
};

// if the following line fails, make sure you use c++20 or newer.
template<typename T> concept Unit = std::is_enum_v<T> && (std::same_as<T, LengthUnit> || std::same_as<T, WeightUnit>);
template<typename T> concept Measurement = std::is_class_v<T> && (std::same_as<T, Length> || std::same_as<T, Weight>);

const std::map<LengthUnit, float> LengthUnitMultiplier = {
    {Kilometer, 1000},
    {Meter, 1},
    {Decimeter, 0.1},
    {Centimeter, 0.01},
    {Millimeter, 0.001},
    {Micrometer, 1e-6},
    {Nanometer, 1e-9}
};
const std::map<WeightUnit, float> WeightUnitMultiplier = {
    {Kilogramm, 1000},
    {Gramm, 1},
    {Milligramm, 1e-3}
};

const std::map<LengthUnit, std::string> LengthUnitString = {
    {Kilometer, "km"},
    {Meter, "m"},
    {Decimeter, "dm"},
    {Centimeter, "cm"},
    {Millimeter, "mm"},
    {Micrometer, "um"},
    {Nanometer, "nm"}
};
const std::map<WeightUnit, std::string> WeightUnitString = {
    {Kilogramm, "kg"},
    {Gramm, "g"},
    {Milligramm, "mg"}
};

template<Unit T>
struct Measure {
    long double measure;
    T unit;
    const std::map<T, std::string>* const stringMap;
    const std::map<T, float>* const multMap;

    Measure(const long double measure, T unit, const std::map<T, std::string>* const stringMap, const std::map<T, float>* const multMap):
        measure(measure), unit(unit), stringMap(stringMap), multMap(multMap) {}
    std::string string() const {
        return std::to_string(measure) + stringMap->at(unit);
    }
};

template<Measurement T> T operator+(const T &lhs, const T &rhs) {
    return T(lhs.measure + rhs.measure * rhs.multMap->at(rhs.unit) / lhs.multMap->at(lhs.unit), lhs.unit);
}
template<Measurement T> T operator-(const T &lhs, const T &rhs) {
    return T(lhs.measure - rhs.measure * rhs.multMap->at(rhs.unit) / lhs.multMap->at(lhs.unit), lhs.unit);
}

struct Length : Measure<LengthUnit> {
    int dimension;
    Length(const long double measure, const LengthUnit unit, const int dimension) : Measure<LengthUnit>(measure, unit, &LengthUnitString, &LengthUnitMultiplier) {
        if (dimension < 1) {
            throw std::invalid_argument("Dimension must be at least 1");
        }
        this->dimension = dimension;
    }
    Length(const long double measure, const LengthUnit unit) : Measure<LengthUnit>(measure, unit, &LengthUnitString, &LengthUnitMultiplier), dimension(1) {}
};

struct Weight : Measure<WeightUnit> {
    Weight(const long double measure, const WeightUnit unit) : Measure<WeightUnit>(measure, unit, &WeightUnitString, &WeightUnitMultiplier) {}
};

Length operator""_km(const long double measure) {
    return Length(measure, Kilometer);
}
Length operator""_m(const long double measure) {
    return Length(measure, Meter);
}
Length operator""_dm(const long double measure) {
    return Length(measure, Decimeter);
}
Length operator""_cm(const long double measure) {
    return Length(measure, Centimeter);
}
Length operator""_mm(const long double measure) {
    return Length(measure, Millimeter);
}
Length operator""_um(const long double measure) {
    return Length(measure, Nanometer);
}
Length operator""_nm(const long double measure) {
    return Length(measure, Nanometer);
}

Weight operator""_kg(const long double measure) {
    return Weight(measure, Kilogramm);
}
Weight operator""_g(const long double measure) {
    return Weight(measure, Gramm);
}
Weight operator""_mg(const long double measure) {
    return Weight(measure, Milligramm);
}

int main() {
    auto test = 0.314_nm;
    std::cout << test.string() << std::endl;
    auto t2 = Length(0.123, Nanometer);
    std::cout << t2.string() << std::endl;
    Length res = test+t2;
    std::cout << res.string() << std::endl;

    return 0;
}
