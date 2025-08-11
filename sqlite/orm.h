#ifndef CPP_DEMO_ORM_H
#define CPP_DEMO_ORM_H

#include <filesystem>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <sqlite3.h>

struct Column;

enum ColumnType {
    Integer,
    Real,
    Text,
};

using DB_Type = std::variant<int, double, std::string>;
std::ostream& operator<<(std::ostream& os, const DB_Type& var);
using DB_Func = DB_Type(*)(sqlite3_stmt*, int);

/*
 * sqlite3-function-proxies for DB_Type support
 */
DB_Type getInt(sqlite3_stmt* stmt, int col);
DB_Type getDouble(sqlite3_stmt* stmt, int col);
DB_Type getString(sqlite3_stmt* stmt, int col);


int bindInt(sqlite3_stmt* stmt, int bindIndex, const DB_Type &newValue);
int bindDouble(sqlite3_stmt* stmt, int bindIndex, const DB_Type &newValue);
int bindString(sqlite3_stmt* stmt, int bindIndex, const DB_Type &newValue);

/*
 * ORM query building blocks
 */

struct SelectConfig {
    std::string whereClause = "";  // yeah, using a string here is really ugly, but make it work first, make it nice later.
    int limit = 0;
    int offset = 0;
};
struct UpdateSet {
    std::string ColumnName;
    DB_Type NewValue;
};
struct UpdateConfig {
    std::string whereClause = "";
    std::vector<UpdateSet> set;
};
struct InsertConfig {
    std::vector<Column> columns;
    std::vector<std::vector<DB_Type>> data;
};

/*
 * ORM table building blocks
 */

struct Column {
    static std::map<ColumnType, DB_Func> selectFuncs;
    std::string name;
    ColumnType type;
    bool mandatory; // for inserts
    Column(const std::string &name, const ColumnType type) : name(name), type(type), mandatory(false) {};
};

struct IntCol : Column {
    explicit IntCol(const std::string &name): Column(name, Integer) {};
};
struct RealCol : Column {
    explicit RealCol(const std::string &name): Column(name, Real) {};
};
struct TextCol : Column {
    explicit TextCol(const std::string &name): Column(name, Text) {};
};

struct HeaderRow {
    std::vector<Column> headers;
    std::string getNames() const;
    ColumnType getColumnType(const std::string &name) const;
};

struct Table {
    const std::string name;
    const HeaderRow header;
    const std::filesystem::path dbFile;
    std::vector<std::vector<DB_Type>> rows;

    Table(const std::filesystem::path &dbPath, const std::string &name, const HeaderRow &header): name(name), header(header), dbFile(dbPath) {}
    Table(const std::string &name, const HeaderRow &header): name(name), header(header), dbFile(std::filesystem::current_path() / ".." / "data.db") {}

    int select(SelectConfig config);
    int update(UpdateConfig config) const;
};

#endif //CPP_DEMO_ORM_H