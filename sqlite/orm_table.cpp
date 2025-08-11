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
    rows.clear();
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

int Table::update(UpdateConfig config) const {
    if (config.set.size() < 1) {
        throw std::invalid_argument("update-statement with nothing to update");
    }
    sqlite3* db;
    sqlite3_stmt* stmt;
    if (const int rc = sqlite3_open(dbFile.c_str(), &db); rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // prepare the update-query-string
    auto sql = "UPDATE "+name+" SET ";
    int bindingPos = 1;
    std::vector<std::tuple<int, DB_Type, ColumnType>> bindings;
    for (auto [colName, newVal] : config.set) {
        bindings.push_back({bindingPos, newVal, header.getColumnType(colName)});
        sql += colName + " = ?,";
        bindingPos++;
    }
    sql.pop_back(); // remove trailing comma
    if (config.whereClause != "") {
        sql += " WHERE " + config.whereClause;
    }
    sql += ";";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    for (auto [pos, val, type] : bindings) {
        switch (type) {
            case Integer:
                if (bindInt(stmt, pos, val) != SQLITE_OK) {
                    std::cerr << "Failed to bind parameter" << std::endl;
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return 1;
                }
                break;
            case Real:
                if (bindDouble(stmt, pos, val) != SQLITE_OK) {
                    std::cerr << "Failed to bind parameter" << std::endl;
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return 1;
                }
                break;
            case Text:
                if (bindString(stmt, pos, val) != SQLITE_OK) {
                    std::cerr << "Failed to bind parameter" << std::endl;
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return 1;
                }
                break;
        }
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
