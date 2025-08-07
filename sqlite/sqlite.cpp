#include <filesystem>
#include <iostream>
#include <sqlite3.h> // "No such file or directory" ->  given you are using a Debian OS: sudo apt install libsqlite3-dev

int main() {
    const std::filesystem::path dbPath = std::filesystem::current_path() / ".." / "data.db";
    const std::string dbPathStr = dbPath.string();

    sqlite3* db;
    sqlite3_stmt* stmt;
    const auto sql = "SELECT n1, n2, n3, n4, n5, n6, super FROM lotto LIMIT ?;";

    int rc = sqlite3_open(dbPathStr.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_bind_int(stmt, 1, 10) != SQLITE_OK) {
        std::cerr << "Failed to bind parameter" << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    while (rc=sqlite3_step(stmt), rc == SQLITE_ROW) {
        const int n1 = sqlite3_column_int(stmt,0);
        std::cout << n1 << std::endl;
    }
    if (rc != SQLITE_DONE) {
        std::cerr << "Error during iteration: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}