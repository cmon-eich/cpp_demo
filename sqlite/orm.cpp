
#include <filesystem>
#include <string>
#include <variant>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>
#include <sqlite3.h>

using DB_Type = std::variant<int, double, std::string>;
std::ostream& operator<<(std::ostream& os, const DB_Type& var) {
    std::visit([&os](const auto& v) { os << v; }, var);
    return os;
}

using DB_ExtractFunc = std::variant<
    std::function<int(sqlite3_stmt*, int)>,
    std::function<double(sqlite3_stmt*, int)>
>;

DB_Type getInt(sqlite3_stmt* stmt, const int col) {
    return sqlite3_column_int(stmt, col);
}

DB_Type getDouble(sqlite3_stmt* stmt, const int col) {
    return sqlite3_column_double(stmt, col);
}


struct SelectConfig {
    std::string whereClause = "";  // yeah, using a string here is really ugly, but make it work first, make it nice later.
    int limit = 0;
    int offset = 0;
};

struct Column {
    std::string name;
    DB_Type(*extractFunc)(sqlite3_stmt*, int);
    Column(const std::string &name, DB_Type(*extract_func)(sqlite3_stmt*, int)): name(name), extractFunc(extract_func) {};
};

struct HeaderRow {
    std::vector<Column> headers;
    std::string getNames() const {
        std::string result = headers[0].name;
        for (int i = 1; i < headers.size(); i++) {
            result += ", " + headers[i].name;
        }
        return result;
    }
};

struct Table {
    const std::string name;
    const HeaderRow header;
    const std::filesystem::path dbFile;
    std::vector<std::vector<DB_Type>> rows;

    Table(const std::string &name, const HeaderRow &header): name(name), header(header), dbFile(std::filesystem::current_path() / "data.db") {}
    Table(const std::filesystem::path &dbPath, const std::string &name, const HeaderRow &header): name(name), header(header), dbFile(dbPath) {}
    int select(SelectConfig config) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int rc = sqlite3_open(dbFile.c_str(), &db);
        std::cout << dbFile << std::endl;
        if (rc != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return 1;
        }

        std::vector<std::tuple<int, int>> bindings;
        int bindingPos = 1; // bindings start with index 1

        auto sql = "SELECT "+header.getNames()+" FROM "+name;
        if (config.whereClause != "") {
            sql += " WHERE " + config.whereClause;
        }
        if (config.limit != 0) {
            sql += " LIMIT ?";
            bindings.push_back(std::tuple<int, int>{bindingPos++, config.limit});
        }
        if (config.offset != 0) {
            sql += " OFFSET ?";
            bindings.push_back(std::tuple<int, int>{bindingPos++, config.offset});
        }
        sql += ";";

        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return 1;
        }
        for (auto [pos, val] : bindings) {
            if (sqlite3_bind_int(stmt, pos, val) != SQLITE_OK) {
                std::cerr << "Failed to bind parameter" << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return 1;
            }
        }

        while (rc=sqlite3_step(stmt), rc == SQLITE_ROW) {
            std::vector<DB_Type> row;
            for (int i = 0; i < header.headers.size(); i++) {
                row.push_back(header.headers[i].extractFunc(stmt, i)); // TODO make save
            }
            rows.push_back(row);
        }
        if (rc != SQLITE_DONE) {
            std::cerr << "Error during iteration: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
};

int main() {
    const std::vector columns = {
        Column("n1", getInt),
        Column("n2", getInt),
        Column("n3", getInt),
        Column("n4", getInt),
        Column("n5", getInt),
        Column("n6", getInt),
        Column("super", getInt),

    };
    const HeaderRow header = {columns};
    auto tbl = Table( "lotto", header);
    tbl.select({"", 10, 0});
    for (auto row : tbl.rows) {
        for (auto col : row) {
            std::cout << std::setw(5) << std::right << col;
        }
        std::cout << std::endl;
    }
}