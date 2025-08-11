#include <iostream>
#include "orm.h"

std::map<ColumnType, DB_Func> Column::selectFuncs = {
    {Integer, getInt},
    {Real, getDouble},
    {Text, getString}
};

int Table::select(SelectConfig config) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc = sqlite3_open(dbFile.c_str(), &db);
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
        bindings.push_back(std::tuple{bindingPos++, config.limit});
    }
    if (config.offset != 0) {
        sql += " OFFSET ?";
        bindings.push_back(std::tuple{bindingPos++, config.offset});
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
            row.push_back(header.headers[i].selectFuncs.at(header.headers[i].type)(stmt, i));
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

int Table::update(UpdateConfig config) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    if (const int rc = sqlite3_open(dbFile.c_str(), &db); rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // SQL with parameter placeholders
    auto sql = "UPDATE "+name+" SET name = ? WHERE id = ?;";

    // Prepare statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // Bind parameters (1-based index)
    sqlite3_bind_text(stmt, 1, "NewName", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, 1); // update user with ID=1

    // Execute statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Row updated successfully." << std::endl;
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
