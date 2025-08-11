#include "orm.h"


std::ostream& operator<<(std::ostream& os, const DB_Type& var) {
    std::visit([&os](const auto& v) { os << v; }, var);
    return os;
}

DB_Type getInt(sqlite3_stmt* stmt, const int col) {
    return sqlite3_column_int(stmt, col);
}
DB_Type getDouble(sqlite3_stmt* stmt, const int col) {
    return sqlite3_column_double(stmt, col);
}
DB_Type getString(sqlite3_stmt* stmt, const int col) {
    return std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, col)));
}

int bindInt(sqlite3_stmt* stmt, const int bindIndex, const DB_Type &newValue) {
    if (const auto newVal = std::get_if<int>(&newValue)) {
        return sqlite3_bind_int(stmt, bindIndex, *newVal);
    }
    throw std::invalid_argument("trying to bind an unsupported type as an integer");
}
int bindDouble(sqlite3_stmt* stmt, const int bindIndex, const DB_Type &newValue) {
    if (const auto newVal = std::get_if<double>(&newValue)) {
        return sqlite3_bind_double(stmt, bindIndex, *newVal);
    }
    throw std::invalid_argument("trying to bind an unsupported type as an integer");
}
int bindString(sqlite3_stmt* stmt, const int bindIndex, const DB_Type &newValue) {
    if (const auto newVal = std::get_if<std::string>(&newValue)) {
        return sqlite3_bind_text(stmt, bindIndex, newVal->c_str(), -1, SQLITE_TRANSIENT);
    }
    throw std::invalid_argument("trying to bind an unsupported type as an integer");
}