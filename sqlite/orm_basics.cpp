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